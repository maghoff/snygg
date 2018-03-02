#!/bin/bash

set -eo pipefail

BASE_URL="https://magnushoff.com/snygg"
PEXE="../build-pnacl/release/src/platform-pnacl/platform-pnacl.pexe"

if [[ $# -ge 1 ]]
then
	BASE_URL="$1"
fi


cd $(dirname "$0")


function zip_copy() {
	local SRC="$1"
	local TRG="$2"

	mkdir -p "$TRG"

	local FILE
	for FILE in "$SRC"/*
	do
		if [ -d "$FILE" ]
		then
			zip_copy "$FILE" "${TRG}${FILE#$SRC}"
		else
			gzip --best --no-name < "$FILE" > "${TRG}${FILE#$SRC}"
		fi
	done
}


pushd ..
./build_pnacl release
popd

npm install

echo "Optimizing js and css"
./optimize.js

STYLECSS_MD5=$(md5sum optimized/style.css | cut -d ' ' -f 1)
APPJS_MD5=$(md5sum optimized/app.js | cut -d ' ' -f 1)
PEXE_MD5=$(md5sum "$PEXE" | cut -d ' ' -f 1)
PEXE_SIZE=$(stat -c %s "$PEXE")


rm -rf deploy
mkdir deploy

zip_copy "../levels" deploy/levels
zip_copy "../skins" deploy/skins

gzip --best --no-name < src/mp.png > "deploy/mp.png"
gzip --best --no-name < src/throbber.svg > "deploy/throbber.svg"
gzip --best --no-name < src/snygg-screenshot.png > "deploy/snygg-screenshot.png"
./mustache.js <( cat <<EOF
{
	"stylecss": "style-$STYLECSS_MD5.css",
	"appjs": "app-$APPJS_MD5.js",
	"manifest": "data:application/x-pnacl,{\"program\":{\"portable\":{\"pnacl-translate\":{\"url\":\"$BASE_URL/snygg-$PEXE_MD5.pexe\",\"optlevel\":0}}}}",
	"pexe_size": "$PEXE_SIZE"
}
EOF
) < src/index.html | gzip --best --no-name > "deploy/index.html"


rm -rf deploy-cached
mkdir deploy-cached

gzip --best --no-name < optimized/style.css > "deploy-cached/style-$STYLECSS_MD5.css"
gzip --best --no-name < optimized/app.js > "deploy-cached/app-$APPJS_MD5.js"
gzip --best --no-name < "$PEXE" > "deploy-cached/snygg-$PEXE_MD5.pexe"


rm -rf package
mkdir package

cp -r ../levels package/
cp -r ../skins package/

cp src/{mp.png,throbber.svg,snygg-screenshot.png} package/
zcat deploy/index.html > package/index.html

cp optimized/style.css "package/style-$STYLECSS_MD5.css"
cp optimized/app.js "package/app-$APPJS_MD5.js"
cp "$PEXE" "package/snygg-$PEXE_MD5.pexe"

tar zcf snygg.tar.xz --transform 's,^package/,snygg/,' package

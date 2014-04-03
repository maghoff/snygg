#!/bin/bash -e

PEXE="../build-pnacl/release/src/platform-pnacl/platform-pnacl.pexe"

cd $(dirname "$0")

pushd ..
./build_pnacl release
popd

npm install

echo "Optimizing js and css"
./optimize.js

STYLECSS_MD5=$(md5sum optimized/style.css | cut -d ' ' -f 1)
APPJS_MD5=$(md5sum optimized/app.js | cut -d ' ' -f 1)
PEXE_MD5=$(md5sum "$PEXE" | cut -d ' ' -f 1)


rm -rf deploy
mkdir deploy

mkdir -p deploy/levels
for x in ../levels/*.lua
do
	gzip --best --no-name < "$x" > "deploy/levels/$(basename $x)"
done

mkdir -p deploy/skins/snakeskin
for x in ../skins/snakeskin/*
do
	gzip --best --no-name < "$x" > "deploy/skins/snakeskin/$(basename $x)"
done

gzip --best --no-name < src/mp.png > "deploy/mp.png"
gzip --best --no-name < src/throbber.svg > "deploy/throbber.svg"
gzip --best --no-name < optimized/style.css > "deploy/style-$STYLECSS_MD5.css"
gzip --best --no-name < optimized/app.js > "deploy/app-$APPJS_MD5.js"
gzip --best --no-name < "$PEXE" > "deploy/snygg-$PEXE_MD5.pexe"

./mustache.js <( cat <<EOF
{
	"stylecss": "style-$STYLECSS_MD5.css",
	"appjs": "app-$APPJS_MD5.js",
	"manifest": "data:application/x-pnacl,{\"program\":{\"portable\":{\"pnacl-translate\":{\"url\":\"http://magnushoff.com/snygg/snygg-$PEXE_MD5.pexe\",\"optlevel\":0}}}}"
}
EOF
) < src/index.html | gzip --best --no-name > "deploy/index.html"

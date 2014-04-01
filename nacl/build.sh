#!/bin/bash -e

cd $(dirname "$0")

npm install

echo "Optimizing js and css"
./optimize.js

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
gzip --best --no-name < src/index.html > "deploy/index.html"
gzip --best --no-name < optimized/style.css > "deploy/style.css"
gzip --best --no-name < optimized/app.js > "deploy/app.js"
gzip --best --no-name < src/manifest.json > "deploy/platform-pnacl.nmf"
gzip --best --no-name < ../build-pnacl/release/src/platform-pnacl/platform-pnacl.pexe > "deploy/platform-pnacl.pexe"

#!/bin/bash -e

S3_TARGET="s3://magnushoff.com/snygg/"

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

gzip --best --no-name < src/index.html > "deploy/index.html"
gzip --best --no-name < src/manifest.json > "deploy/platform-pnacl.nmf"
gzip --best --no-name < ../build-pnacl/release/src/platform-pnacl/platform-pnacl.pexe > "deploy/platform-pnacl.pexe"

s3cmd sync \
	--guess-mime-type \
	--no-mime-magic \
	--cf-invalidate \
	--delete-removed \
	--add-header "Content-Encoding: gzip" \
	"deploy/" "$S3_TARGET"

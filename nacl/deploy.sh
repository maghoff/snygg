#!/bin/bash -e

S3_TARGET="s3://magnushoff.com/snygg/"

./build.sh

s3cmd sync \
	--guess-mime-type \
	--no-mime-magic \
	--cf-invalidate \
	--add-header "Content-Encoding: gzip" \
	"deploy/" "$S3_TARGET"

s3cmd sync \
	--guess-mime-type \
	--no-mime-magic \
	--add-header "Content-Encoding: gzip" \
	--add-header "Cache-Control: public, max-age=31536000" \
	"deploy-cached/" "$S3_TARGET"

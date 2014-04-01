#!/bin/bash -e

S3_TARGET="s3://magnushoff.com/snygg/"

./build.sh

s3cmd sync \
	--guess-mime-type \
	--no-mime-magic \
	--cf-invalidate \
	--delete-removed \
	--add-header "Content-Encoding: gzip" \
	"deploy/" "$S3_TARGET"

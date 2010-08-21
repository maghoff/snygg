#!/bin/bash

case "$(uname)" in
Linux) ./prepare-ubuntu.sh ;;
Darwin) ./prepare-mac.sh ;;
*) echo "A preparescript has not been written for this platform" ;;
esac

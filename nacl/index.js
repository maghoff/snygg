#!/usr/bin/env node

var resource_tree = require('resource-tree');
var path = require('path');

root = path.join(__dirname, '..');

var directoryResourceFactory = function (fullpath) { return new resource_tree.DirectoryResource(fullpath); };

server = resource_tree.createServer(new resource_tree.SerialLookup([
	{
		"platform-pnacl.nmf": new resource_tree.FileResource(path.join(root, "nacl/src/manifest.json"), {'content-type': 'application/octet-stream'}),
		"platform-pnacl.pexe": new resource_tree.FileResource(path.join(root, "build-pnacl/debug/src/platform-pnacl/platform-pnacl.pexe"), {'content-type': 'application/octet-stream'}),
		"levels": new resource_tree.FileLookup(path.join(root, "levels"))
	},
	new resource_tree.FileLookup(path.join(root, "nacl", "src")),
]));

server.on("listening", function () {
	console.log(server.address())
});

server.listen(0);

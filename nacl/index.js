#!/usr/bin/env node

var resource_tree = require('resource-tree');
var path = require('path');
var child_process = require('child_process');
var minimist = require('minimist');

var argv = minimist(process.argv);

var root = path.join(__dirname, '..');
var directoryResourceFactory = function (fullpath) { return new resource_tree.DirectoryResource(fullpath); };

server = resource_tree.createServer(new resource_tree.SerialLookup([
	{
		"platform-pnacl.nmf": new resource_tree.FileResource(path.join(root, "nacl/src/manifest.json"), {'content-type': 'application/octet-stream'}),
		"platform-pnacl.pexe": new resource_tree.FileResource(path.join(root, "build-pnacl/debug/src/platform-pnacl/platform-pnacl.pexe"), {'content-type': 'application/octet-stream'}),
		"levels": new resource_tree.FileLookup(path.join(root, "levels")),
		"skins": new resource_tree.FileLookup(path.join(root, "skins"))
	},
	new resource_tree.FileLookup(path.join(root, "nacl", "src")),
]));

server.on("listening", function () {
	var url = 'http://localhost:' + server.address().port + '/index.html';
	console.log(url);
	if (argv.run) {
		var commands = Array.isArray(argv.run) ? argv.run : [argv.run];
		commands.forEach(function (command) {
			var to_run = command.replace('%u', url);
			console.log("Executing", to_run);
			child_process.exec(to_run);
		});
	}
});

server.listen(0);

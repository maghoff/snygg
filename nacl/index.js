#!/usr/bin/env node

var resource_tree = require('resource-tree');
var path = require('path');
var child_process = require('child_process');
var minimist = require('minimist');

var argv = minimist(process.argv);

var root = path.join(__dirname, '..');
var directoryResourceFactory = function (fullpath) { return new resource_tree.DirectoryResource(fullpath); };

var variant = "debug";
if (argv.release) variant = "release";

server = resource_tree.createServer({
	"index.html": new resource_tree.FileResource(path.join(root, "nacl/src/index.html")),
	"platform-pnacl.nmf": new resource_tree.FileResource(path.join(root, "nacl/src/manifest.json")),
	"platform-pnacl.pexe": new resource_tree.FileResource(path.join(root, "build-pnacl/" + variant + "/src/platform-pnacl/platform-pnacl.pexe")),
	"levels": new resource_tree.FileLookup(path.join(root, "levels")),
	"skins": new resource_tree.FileLookup(path.join(root, "skins"))
});

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

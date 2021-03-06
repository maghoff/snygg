#!/usr/bin/env node

var resource_tree = require('resource-tree');
var MustacheResource = require('./mustache-resource');
var path = require('path');
var child_process = require('child_process');
var minimist = require('minimist');

var argv = minimist(process.argv);

var root = path.join(__dirname, '..');
var directoryResourceFactory = function (fullpath) { return new resource_tree.DirectoryResource(fullpath); };

if (argv.h || argv.help) {
	console.error("Usage: ./dev.js [--release] [--hostname <hostname>]");
	process.exit(1);
}

var variant = "debug";
if (argv.release) variant = "release";

var hostname = argv.hostname || "localhost";

var manifest = {
	"program": {
		"portable": {
			"pnacl-translate": {
				"url": "",
				"optlevel": 0
			}
		}
	}
};

var indexView = {
	stylecss: "style.css",
	appjs: "app.js",
	manifest: ""
};

server = resource_tree.createServer(new resource_tree.SerialLookup([
	{
		"index.html": new MustacheResource(path.join(root, "nacl/src/index.html"), indexView),
		"style.css": new resource_tree.FileResource(path.join(root, "nacl/src/style.css")),
		"app.js": new resource_tree.FileResource(path.join(root, "nacl/node_modules/requirejs/require.js")),
		"mp.png": new resource_tree.FileResource(path.join(root, "nacl/src/mp.png")),
		"throbber.svg": new resource_tree.FileResource(path.join(root, "nacl/src/throbber.svg")),
		"platform-pnacl.pexe": new resource_tree.FileResource(path.join(root, "build-pnacl/" + variant + "/src/platform-pnacl/platform-pnacl.pexe")),
		"levels": new resource_tree.FileLookup(path.join(root, "levels")),
		"skins": new resource_tree.FileLookup(path.join(root, "skins")),
		"deps": new resource_tree.FileLookup(path.join(root, "nacl/deps"))
	},
	new resource_tree.FileLookup(path.join(root, "nacl/src"))
]));

server.on("listening", function () {
	var url = 'http://' + hostname + ':' + server.address().port + '/index.html';
	manifest.program.portable["pnacl-translate"].url = 'http://' + hostname + ':' + server.address().port + '/platform-pnacl.pexe';
	indexView.manifest = "data:application/x-pnacl," + JSON.stringify(manifest);

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

#!/usr/bin/env node

var fs = require('fs');
var Mustache = require('mustache');

fs.readFile(process.argv[2], "utf-8", function (err, viewText) {
	if (err) {
		console.error(err);
		process.exit(1);
	}

	var view = JSON.parse(viewText);

	fs.readFile("/dev/stdin", "utf-8", function (err, template) {
		if (err) {
			console.error(err);
			process.exit(1);
		}

		process.stdout.write(Mustache.render(template, view));
	});
});

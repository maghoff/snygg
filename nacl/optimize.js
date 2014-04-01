#!/usr/bin/env node

var async = require('async');
var requirejs = require('requirejs');

var configs = [
	{
		id: 'js',
		baseUrl: 'src',
		name: 'snygg',
		out: 'optimized/app.js',
		include: 'requireLib',
		paths: {
			"requireLib": "../node_modules/requirejs/require"
		},
		mainConfigFile: "src/snygg.js",
		preserveLicenseComments: false
	}, {
		id: 'css',
		cssIn: 'src/style.css',
		out: 'optimized/style.css'
	}
];

function requirejsOptimize(config, callback) {
	requirejs.optimize(
		config,
		function (buildResponse) { callback(null, config); },
		function (err) { callback(err); }
	);
}

async.map(configs, requirejsOptimize, function (err, res) {
	if (err) {
		console.error(err);
		process.exit(1);
	}
});

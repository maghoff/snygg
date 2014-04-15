#!/usr/bin/env node

var fs = require('fs');
var uglifyjs = require('uglify-js');
var ajax = require('pouchdb').ajax;

var couchHost = "mag.cloudant.com";
var dbs = ["_users", "snygg", "personal_records"];


function mapFunction(orig) {
	return uglifyjs.minify(orig.toString(), {fromString: true}).code.replace(/^function[^(]*\(/, 'function(');
}

function mapObject(orig) {
	var obj = {};
	for (var field in orig) {
		obj[field] = mapValue(orig[field]);
	}
	return obj;
}

function mapArray(orig) {
	var arr = [];
	for (var index = 0; index < orig.length; ++index) {
		arr[index] = mapValue(orig[index]);
	}
	return arr;
}

function mapValue(orig) {
	if (orig instanceof Function) return mapFunction(orig);
	else if (Array.isArray(orig)) return mapArray(orig);
	else if (typeof orig === 'object') return mapObject(orig);
	else return orig;
}

function prepareDDoc(ddocOrig) {
	return JSON.stringify(mapValue(ddocOrig));
}


function encodeQueryArgs(opts) {
	var queryArgs = "";
	for (key in opts) {
		queryArgs += "&" + encodeURIComponent(key) + "=" + encodeURIComponent(opts[key])
	}
	return queryArgs.substr(1);
}


function processDb(apiKey, db) {
	var docs = require("./docs-" + db);
	for (var doc in docs) docs[doc] = mapValue(docs[doc]);

	var args = { keys: JSON.stringify(Object.keys(docs)) };

	var couch = "https://" + apiKey.key + ":" + apiKey.password + "@" + couchHost + "/";

	ajax({
		url: couch + db + "/_all_docs?" + encodeQueryArgs(args)
	}, function (err, res) {
		if (err) {
			console.error(err);
			return;
		}
		res.rows.forEach(function (row) {
			if (row.value) docs[row.key]._rev = row.value.rev;
		});

		var updates = [];
		for (var doc in docs) {
			docs[doc]._id = doc;
			updates.push(docs[doc]);
		}

		ajax({
			method: "POST",
			url: couch + db + "/_bulk_docs",
			body: { docs: updates }
		}, function (err, res) {
			console.log(db, err, res);
		});
	});
}

var apiKey = JSON.parse(fs.readFileSync('api-key.json', 'utf-8'));

dbs.forEach(function (db) { processDb(apiKey, db); });

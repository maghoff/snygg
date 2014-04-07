#!/usr/bin/env node

var PouchDb = require('pouchdb');

var dbUrl = 'https://mag.cloudant.com/snygg/';
var db = new PouchDb(dbUrl);

var user = "ate";

db.query(
	"snygg/personal_records",
	{
		reduce: false,
		startkey: [ user ],
		endkey: [ user, {} ],
		inclusive_end: false,
		include_docs: true
	},
	function (err, res) {
		if (err) {
			console.error(err);
			process.exit(1);
		}
		console.log("Deleting", res.total_rows, "document" + (res.total_rows === 1 ? "" : "s"));
		//console.log(res.rows.map(function (row) { return row.doc; }));
		var data = {
			docs: res.rows.map(function (row) {
					return {
						_id: row.doc._id,
						_rev: row.doc._rev,
						_deleted: true
					};
				})
		};

		PouchDb.ajax({
			method: "POST",
			url: dbUrl + "_bulk_docs",
			body: JSON.stringify(data)
		}, function (err, res) {
			if (err) {
				console.error(err);
				process.exit(1);
			}
			console.log(res);
		});
	}
);

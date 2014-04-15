var util = require('./util');
var snygg = require('../docs-snygg');

var valid_doc = {
	_id: "id",
	_rev: "0-rev",
	name: "user",
	board: "board",
	timestamp: "2014-01-01T00:00:00.000Z",
	score: 5
};

var logged_in_user_ctx = {
	db: "snygg",
	name: "user",
	roles: []
};


exports["test Must be logged in"] = function (beforeExit, assert) {
	util.expectException(assert, "unauthorized", function () {
		snygg["_design/snygg"].validate_doc_update(valid_doc, null, {db:"snygg",name:null,roles:[]}, null);
	});
};

exports["test Must be correct user"] = function (beforeExit, assert) {
	util.expectException(assert, "unauthorized", function () {
		snygg["_design/snygg"].validate_doc_update(valid_doc, null, {db:"snygg",name:"notuser",roles:[]}, null);
	});
};

exports["test Can insert valid doc"] = function (beforeExit, assert) {
	snygg["_design/snygg"].validate_doc_update(valid_doc, null, logged_in_user_ctx, null);
};

exports["test Valid doc must have name"] = function (beforeExit, assert) {
	util.expectException(assert, "forbidden", function () {
		var doc = util.clone(valid_doc);
		delete doc.name;
		snygg["_design/snygg"].validate_doc_update(doc, null, logged_in_user_ctx, null);
	})
};

exports["test Valid doc must have timestamp"] = function (beforeExit, assert) {
	util.expectException(assert, "forbidden", function () {
		var doc = util.clone(valid_doc);
		delete doc.timestamp;
		snygg["_design/snygg"].validate_doc_update(doc, null, logged_in_user_ctx, null);
	})
};

exports["test Valid doc must have board"] = function (beforeExit, assert) {
	util.expectException(assert, "forbidden", function () {
		var doc = util.clone(valid_doc);
		delete doc.board;
		snygg["_design/snygg"].validate_doc_update(doc, null, logged_in_user_ctx, null);
	});
};

exports["test Valid doc must have score"] = function (beforeExit, assert) {
	util.expectException(assert, "forbidden", function () {
		var doc = util.clone(valid_doc);
		delete doc.score;
		snygg["_design/snygg"].validate_doc_update(doc, null, logged_in_user_ctx, null);
	});
};

exports["test Timestamp must match RFC3339"] = function (beforeExit, assert) {
	util.expectException(assert, "forbidden", function () {
		var doc = util.clone(valid_doc);
		doc.timestamp = "2014-01-01 00:00:00.000Z";
		snygg["_design/snygg"].validate_doc_update(doc, null, logged_in_user_ctx, null);
	});
};

exports["test Timestamp must be UTC with Z"] = function (beforeExit, assert) {
	util.expectException(assert, "forbidden", function () {
		var doc = util.clone(valid_doc);
		doc.timestamp = "2014-01-01T00:00:00.000";
		snygg["_design/snygg"].validate_doc_update(doc, null, logged_in_user_ctx, null);
	});
};

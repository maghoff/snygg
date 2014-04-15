var util = require('./util');
var users = require('../docs-_users');

var valid_doc = {
	_id: "org.couchdb.user:user",
	_rev: "0-rev",
	type: "user",
	name: "user",
	roles: []
};

var anonymous_user_ctx = {
	db: "_users",
	roles: []
};

var logged_in_user_ctx = {
	db: "_users",
	name: "user",
	roles: []
};


exports["test Can create new user anonymously"] = function (beforeExit, assert) {
	users["_design/snygg"].validate_doc_update(valid_doc, null, anonymous_user_ctx, null);
};

exports["test Can add gravatar"] = function (beforeExit, assert) {
	var doc = util.clone(valid_doc);
	doc._rev = "1-rev";
	doc.gravatar = "asfd";
	users["_design/snygg"].validate_doc_update(doc, valid_doc, logged_in_user_ctx, null);
};

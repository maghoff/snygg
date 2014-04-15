module.exports = {
	"_design/snygg": {
		"views": {
			"highscores": {
				"map": function map(doc) {
					if (doc.name && doc.board && doc.score) {
						emit(
							[doc.board, -doc.score, doc.name, doc.timestamp || {}],
							{name: doc.name, score: doc.score, timestamp: doc.timestamp || {}}
						);
					}
				},
				"reduce": function reduce(keys, values, rereduce) {
					function lexicographic_compare(a, b) { return a < b ? a : b; }
					var timestamps = values.map(function (v) { return v.timestamp; });
					var firstTimestamp = timestamps.reduce(lexicographic_compare);

					return {
						name: values[0].name,
						score: values[0].score,
						timestamp: firstTimestamp
					};
				}
			},
			"personal_records": {
				"map": function map(doc) {
					if (doc.board && doc.name && doc.score)
						emit([doc.name, doc.board], [-doc.score, doc.timestamp]);
				},
				"reduce": function reduce(keys, values, rereduce) {
					function compareLists(a, b) {
						if (a[0] !== b[0]) return a[0] < b[0];
						return a[1] < b[1];
					}
					function compare(a, b) { return compareLists(a, b) ? a : b; }
					return values.reduce(compare);
				},
				"dbcopy": "personal_records"
			}
		},
		"validate_doc_update": function validate_doc_update(newDoc, oldDoc, userCtx, secObj) {
			function is_server_or_database_admin(userCtx, secObj) {
				// see if the user is a server admin
				if(userCtx.roles.indexOf('_admin') !== -1) {
					return true; // a server admin
				}

				// see if the user a database admin specified by name
				if(secObj && secObj.admins && secObj.admins.names) {
					if(secObj.admins.names.indexOf(userCtx.name) !== -1) {
						return true; // database admin
					}
				}

				// see if the user a database admin specified by role
				if(secObj && secObj.admins && secObj.admins.roles) {
					var db_roles = secObj.admins.roles;
					for(var idx = 0; idx < userCtx.roles.length; idx++) {
						var user_role = userCtx.roles[idx];
						if(db_roles.indexOf(user_role) !== -1) {
							return true; // role matches!
						}
					}
				}
				return false; // default to no admin
			}

			// admin can do whatever
			if (is_server_or_database_admin(userCtx, secObj)) return;

			if (newDoc._deleted) throw({unauthorized:"Only admin can delete stuff"});

			if (typeof newDoc.name !== 'string') throw({forbidden:".name must be a string"});
			if (newDoc.name !== userCtx.name) throw({unauthorized:".name must match logged in user"});
			if (typeof newDoc.score !== 'number') throw({forbidden:".score must be a number"});
			if (newDoc.score <= 0) throw({forbidden:".score must be more than 0"});
			if (typeof newDoc.board !== 'string') throw({forbidden:".board must be a string"});
			if (newDoc.board.length > 50) throw({forbidden:".board can not be more than 50 characters"});

			var rfc3339pattern = /^\d{4}-\d\d-\d\dT\d\d:\d\d:\d\d(.\d+)?Z$/;
			if (typeof newDoc.timestamp !== 'string') throw({forbidden:".timestamp must be a string"});
			if (!newDoc.timestamp.match(rfc3339pattern)) throw({forbidden:".timestamp must be a valid RFC3339 timestamp in Z timezone"});

			var permissibleFields = ["_id", "_rev", "_revisions", "name", "board", "score", "timestamp"];
			for (var field in newDoc) {
				if (permissibleFields.indexOf(field) === -1) {
					throw({forbidden: 'May not contain field: ' + field});
				}
			}
		},
		"language": "javascript"
	}
};

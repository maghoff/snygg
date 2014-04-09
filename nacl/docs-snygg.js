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
		"language": "javascript"
	}
};

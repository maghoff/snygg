module.exports = {
	"_design/snygg": {
		"views": {
			"highscore_lists": {
				"map": function map(doc) {
					emit(
						[doc.key[1], doc.value[0], doc.value[1]],
						[doc.key[0], -doc.value[0], doc.value[1]]
					);
				}
			}
		},
		"filters": {
			"highscores": function highscores(doc, req) {
				return
					doc.key && req.query.board && doc.key[1] === req.query.board &&
					(!req.query.over || (-doc.value[0] > parseInt(req.query.over, 10)));
			}
		},
		"language": "javascript"
	}
};

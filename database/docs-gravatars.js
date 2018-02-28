module.exports = {
	"_design/snygg": {
		"views": {
			"gravatars": {
				"map": function map(doc) {
					emit(doc.key, doc.value);
				}
			}
		},
		"language": "javascript"
	}
};

exports.clone = function (doc) {
	return JSON.parse(JSON.stringify(doc));
};

exports.expectException = function (assert, exception, f) {
	try {
		f();
	}
	catch (err) {
		assert.isDefined(err[exception], "Expected " + JSON.stringify(exception) + ", got " + JSON.stringify(err));
		return;
	}
	assert.fail("Missing expected exception of type " + exception);
};


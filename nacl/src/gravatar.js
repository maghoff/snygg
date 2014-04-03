define(['deps/md5'], function () {
	return function (email) {
		var normalized = email.trim().toLowerCase();
		return md5(normalized);
	};
});

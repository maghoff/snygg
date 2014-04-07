define([], function () {
	return function (opts) {
		var queryArgs = "";
		for (key in opts) {
			queryArgs += "&" + encodeURIComponent(key) + "=" + encodeURIComponent(opts[key])
		}
		return queryArgs.substr(1);
	}
});

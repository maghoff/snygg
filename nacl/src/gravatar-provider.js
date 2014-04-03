define([
	'ajax',
	'gravatar',
], function (
	ajax,
	gravatarIdFromEmail
) {
	function GravatarProvider(centralCouch) {
		if (!this instanceof GravatarProvider) return new GravatarProvider(centralCouch);

		this.centralCouch = centralCouch;
		this.gravatars = {};
		this.pendingCallbacks = {};
		this.currentlyFetching = [];
		this.pendingAjaxCall = false;
	}

	GravatarProvider.prototype.buildGravatarUrl = function (name) {
		if (this.gravatars[name] === "mm") {
			return "http://www.gravatar.com/avatar/?d=mm&f=y";
		} else if (!this.gravatars[name]) {
			return "http://www.gravatar.com/avatar/" + gravatarIdFromEmail(name) + "?d=retro&f=y";
		} else {
			return "http://www.gravatar.com/avatar/" + this.gravatars[name] + "?d=retro";
		}
	};

	GravatarProvider.prototype.fetchMissing = function () {
		this.pendingAjaxCall = false;

		var needs = [];
		for (var name in this.pendingCallbacks) {
			if (this.currentlyFetching.indexOf(name) === -1) {
				needs.push(name);
				this.currentlyFetching.push(name);
			}
		}

		if (needs.length === 0) return;

		ajax({
			url: this.centralCouch + "_users/_design/snygg/_view/gravatars?keys=" + encodeURIComponent(JSON.stringify(needs))
		}, function (err, data) {
			if (err) {
				console.error(err);
				needs.forEach(function (name) {
					this.gravatars[name] = "mm";
				}.bind(this));
			} else {
				data.rows.forEach(function (row) {
					this.gravatars[row.key] = row.value;
				}.bind(this));
			}
			needs.forEach(function (name) {
				var gravatarUrl = this.buildGravatarUrl(name);
				this.pendingCallbacks[name].forEach(function (callback) {
					callback(gravatarUrl);
				});
				delete this.pendingCallbacks[name];
			}.bind(this));
		}.bind(this));
	};

	GravatarProvider.prototype.get = function (name, callback) {
		if (this.gravatars.hasOwnProperty(name)) {
			callback(this.buildGravatarUrl(name));
		} else {
			this.pendingCallbacks[name] = this.pendingCallbacks[name] || [];
			this.pendingCallbacks[name].push(callback);
			if (this.currentlyFetching.indexOf(name) === -1) {
				if (!this.pendingAjaxCall) {
					this.pendingAjaxCall = true;
					setTimeout(this.fetchMissing.bind(this), 0);
				}
			}
		}
	};

	return GravatarProvider;
});

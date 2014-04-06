define([
	'ajax',
	'gravatar',
	'deps/sha1',
], function (
	ajax,
	gravatarIdFromEmail
) {
	function getElementsByNames(root, names) {
		var elements = {};
		names.forEach(function (name) {
			elements[name] = root.querySelector("[name="+name+"]");
		});
		return elements;
	}

	function hashPassword(password, salt) {
		return CryptoJS.SHA1(password + salt).toString();
	}


	function Login(dom, centralCouch, listener) {
		if (!this instanceof Login) return new Login(dom, centralCouch, listener);

		this.dom = dom;
		this.listener = listener;
		this.centralCouch = centralCouch;

		this.sessionUrl = this.centralCouch + "_session";

		this.readLoggedInStatus();

		var register = this.dom.getElementsByClassName("register")[0];
		var login = this.dom.getElementsByClassName("login")[0];
		var logout = this.dom.getElementsByClassName("logout")[0];

		register.addEventListener("click", this.doRegister.bind(this));
		login.addEventListener("click", this.doLoginButton.bind(this));
		logout.addEventListener("click", this.doLogout.bind(this));
	}

	Login.prototype.doing = function (thing) {
		this.dom.getElementsByClassName("doing-message")[0].textContent = thing;
		this.dom.classList.add("doing");
		this.dom.classList.remove("status");
	};

	Login.prototype.done = function () {
		this.dom.classList.remove("doing");
	};

	Login.prototype.status = function (msg) {
		this.dom.getElementsByClassName("status-message")[0].textContent = msg;
		this.dom.classList.add("status");
	};

	Login.prototype.showNotLoggedIn = function () {
		this.dom.classList.add("not-logged-in");
		this.dom.classList.remove("logged-in");
	};

	Login.prototype.showLoggedIn = function (doc) {
		this.dom.getElementsByClassName("logged-in-user")[0].textContent = doc.name;

		var loggedIn = this.dom.getElementsByClassName("logged-in-form")[0];

		var gravatar;
		if (doc.gravatar) gravatar = "http://www.gravatar.com/avatar/" + doc.gravatar + "?d=retro&s=200";
		else gravatar = "http://www.gravatar.com/avatar/" + gravatarIdFromEmail(doc.name) + "?d=retro&f=y&s=200";
		loggedIn.getElementsByClassName("gravatar")[0].setAttribute("src", gravatar);

		loggedIn.classList.remove("has-gravatar");
		if (doc.gravatar) loggedIn.classList.add("has-gravatar");

		this.dom.classList.remove("not-logged-in");
		this.dom.classList.add("logged-in");
	};

	Login.prototype.getUserDetails = function (username) {
		ajax({
			"method": "GET",
			"url": this.centralCouch + "_users/org.couchdb.user:" + username
		}, function (err, doc) {
			this.done();
			if (err) {
				console.error(err);
				this.status("Unknown error logging in. Please try again later");
			} else {
				this.showLoggedIn(doc);
				this.listener.loggedIn(username);
			}
		}.bind(this));
	};

	Login.prototype.readLoggedInStatus = function () {
		this.doing("Logging in");

		ajax({
			"method": "GET",
			"url": this.sessionUrl
		}, function (err, doc) {
			this.done();
			if (err) {
				console.error(err);
				this.status("Error with login system");
				return;
			}

			if (!doc.userCtx.name) {
				this.showNotLoggedIn();
			} else {
				this.getUserDetails(doc.userCtx.name);
			}
		}.bind(this));
	};

	Login.prototype.doLogin = function (username, password) {
		ajax({
			"method": "POST",
			"url": this.centralCouch + "_session",
			"headers": {
				"Content-Type": "application/x-www-form-urlencoded"
			},
			"body": "name=" + encodeURIComponent(username) + "&password=" + encodeURIComponent(password)
		}, function (err, doc) {
			if (err && err.status === 403) {
				this.done();
				this.status("Invalid username or password");
			} else if (err) {
				this.done();
				console.error(err);
				this.status("Unknown error logging in. Please try again later");
			} else {
				this.getUserDetails(doc.name);
			}
		}.bind(this));
	};

	Login.prototype.doLoginButton = function () {
		this.doing("Logging in");
		var inputs = getElementsByNames(this.dom, ["username", "password"]);
		this.doLogin(inputs.username.value, inputs.password.value);
	};

	Login.prototype.doRegister = function () {
		var inputs = getElementsByNames(this.dom, ["username", "email", "password"]);
		var password = inputs.password.value;
		var salt = CryptoJS.lib.WordArray.random(128/8).toString();
		var newUser = {
			"_id": "org.couchdb.user:" + inputs.username.value,
			"name": inputs.username.value,
			"gravatar": inputs.email.value ? gravatarIdFromEmail(inputs.email.value) : "",
			"type": "user",
			"roles": [],
			"salt": salt,
			"password_sha": hashPassword(password, salt)
		};

		this.doing("Registring user");
		ajax({
			"method": "POST",
			"url": this.centralCouch + "_users",
			"body": newUser
		}, function (err, doc) {
			if (err && err.status === 409) {
				this.done();
				this.status("User ID " + JSON.stringify(newUser.name) + " already taken");
			} else if (err) {
				this.done();
				console.error(err);
				this.status("Unknown error registring user. Please try again later");
			} else {
				this.doLogin(newUser.name, password);
			}
		}.bind(this));
	};

	Login.prototype.doLogout = function () {
		this.doing("Logging out");
		ajax({
			"method": "DELETE",
			"url": this.sessionUrl
		}, function (err, data) {
			this.done();
			if (err) {
				console.error(err);
				this.status("Unknown error logging out. Please try again later");
				return;
			}
			this.showNotLoggedIn();
			this.listener.loggedOut();
		}.bind(this));
	};

	return Login;
});

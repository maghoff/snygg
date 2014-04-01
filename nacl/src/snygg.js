var centralCouch = 'https://mag.cloudant.com/'; // FIXME Global data :(

function initializeDb() {
	var highscores = new PouchDB('highscores', { cache: true });
	var localHighscores = new PouchDB('localHighscores');

	var remoteDatabase = centralCouch + "snygg/";

	function syncError() {
		console.log("syncError");
	}

	function sync() {
		var opts = { continuous: true, complete: syncError, cache:true };
		highscores.replicate.to(remoteDatabase, opts);
	}

	sync();

	return {
		"highscores": highscores,
		"localHighscores": localHighscores
	};
}


function getElementsById(doc, ids) {
	var elements = {};
	ids.forEach(function (id) {
		elements[id] = doc.getElementById(id);
	});
	return elements;
}


function doing(thing) {
	document.getElementById("doing-message").textContent = thing;
	document.getElementById("login-interaction").classList.add("doing");
	document.getElementById("login-interaction").classList.remove("status");
}

function done() {
	document.getElementById("login-interaction").classList.remove("doing");
}

function status(msg) {
	document.getElementById("status-message").textContent = msg;
	document.getElementById("login-interaction").classList.add("status");
}


function gravatarIdFromEmail(email) {
	var normalized = email.trim().toLowerCase();
	return md5(normalized);
}


function initializeLogin(db) {
	var ajax = PouchDB.ajax;

	var sessionUrl = centralCouch + "_session";

	var register = document.getElementById("register");
	var login = document.getElementById("login");
	var logout = document.getElementById("logout");

	var highscoresMapper = null;

	doing("Logging in");
	ajax({
		"method": "GET",
		"url": sessionUrl
	}, function (err, doc) {
		done();
		if (err) {
			console.log(err);
			status("Error with login system");
			return;
		}

		if (!doc.userCtx.name) {
			showNotLoggedIn();
		} else {
			getUserDetails(doc.userCtx.name);
		}
	});

	function getUserDetails(username) {
		ajax({
			"method": "GET",
			"url": centralCouch + "_users/org.couchdb.user:" + username
		}, function (err, doc) {
			done();
			if (err) {
				status("Unknown error logging in. Please try again later");
			} else {
				showLoggedIn(doc);
				highscoresMapper = db.localHighscores.changes({
					continuous: true,
					include_docs: true,
					onChange: function (change) {
						var doc = change.doc;
						if (doc._deleted) return;

						db.highscores.put({
							_id: doc._id,
							name: username,
							board: doc.board,
							score: doc.score
						}, function (err, status) {
							if (err && err.status !== 409) return console.error(err);
							db.localHighscores.remove(doc);
						});
					}
				});
			}
		});
	}

	function doLogin(username, password) {
		ajax({
			"method": "POST",
			"url": centralCouch + "_session",
			"headers": {
				"Content-Type": "application/x-www-form-urlencoded"
			},
			"body": "name=" + encodeURIComponent(username) + "&password=" + encodeURIComponent(password)
		}, function (err, doc) {
			if (err) {
				done();
				console.error(err);
				status("Unknown error logging in. Please try again later");
			} else {
				getUserDetails(doc.name);
			}
		});
	}

	function hashPassword(password, salt) {
		return CryptoJS.SHA1(password + salt).toString();
	}

	register.addEventListener("click", function (ev) {
		var inputs = getElementsById(document, ["username", "email", "password"]);
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

		doing("Registring user");
		ajax({
			"method": "POST",
			"url": centralCouch + "_users",
			"body": newUser
		}, function (err, doc) {
			if (err && err.status === 409) {
				done();
				status("User ID " + JSON.stringify(newUser.name) + " already taken");
			} else if (err) {
				done();
				console.error(err);
				status("Unknown error registring user. Please try again later");
			} else {
				doLogin(newUser.name, password);
			}
		});
	});

	login.addEventListener("click", function (ev) {
		doing("Logging in");
		var inputs = getElementsById(document, ["username", "password"]);
		doLogin(inputs.username.value, inputs.password.value);
	});

	logout.addEventListener("click", function (ev) {
		doing("Logging out");
		ajax({
			"method": "DELETE",
			"url": sessionUrl
		}, function (err, data) {
			done();
			if (err) {
				console.error(err);
				status("Unknown error logging out. Please try again later");
				return;
			}
			showNotLoggedIn();
			if (highscoresMapper) highscoresMapper.cancel();
			highscoresMapper = null;
		});
	});
}


function showNotLoggedIn() {
	document.getElementById("login-interaction").classList.add("not-logged-in");
	document.getElementById("login-interaction").classList.remove("logged-in");
}

function showLoggedIn(doc) {
	document.getElementById("logged-in-user").textContent = doc.name;

	var gravatar;
	if (doc.gravatar) gravatar = "http://www.gravatar.com/avatar/" + doc.gravatar + "?d=retro";
	else gravatar = "http://www.gravatar.com/avatar/" + gravatarIdFromEmail(doc.name) + "?d=retro&f=y";
	document.getElementById("gravatar").setAttribute("src", gravatar);

	document.getElementById("login-interaction").classList.remove("not-logged-in");
	document.getElementById("login-interaction").classList.add("logged-in");
}


function installApp() {
	var db = initializeDb();

	initializeLogin(db);

	var boardSelector = document.getElementById("board");

	function loadBoard() {
		var board = window.location.hash.substr(1);
		if (!boardSelector.namedItem(board)) {
			if (board) {
				window.history.pushState(null, "Snygg: Pill", "#pill");
			}
			board = "pill";
		}
		boardSelector.namedItem(board).setAttribute("selected", "selected");
		if (snygg) snygg.postMessage(board);
	}
	loadBoard();
	window.addEventListener('hashchange', loadBoard);

	boardSelector.addEventListener('change', function () {
		var boardItem = boardSelector.options[boardSelector.selectedIndex];
		window.history.pushState(
			null,
			"Snygg: " + boardItem.name,
			"#" + boardItem.value
		);
		loadBoard();
	});


	var snygg = document.getElementById("snygg");

	snygg.addEventListener('loadstart', function () {
		document.getElementById('statusField').textContent = "loading";
	});

	snygg.addEventListener('progress', function (ev) {
		var status = "loading";
		if (ev.lengthComputable) {
			var progressPercentage = ev.loaded / ev.total * 100;
			status = "loading<br/><span class='progressbar'><span class='progress' style='width: " + progressPercentage + "%'></span></span>";
		}
		document.getElementById('statusField').innerHTML = status;
	});

	snygg.addEventListener('load', function () {
		document.getElementById('statusField').textContent = "loading";
		loadBoard();
	});

	function displayLastError() {
		document.getElementById('statusField').textContent = snygg.lastError;
	}

	snygg.addEventListener('error', displayLastError);
	snygg.addEventListener('abort', displayLastError);

	snygg.addEventListener('crash', function () {
		var status;
		if (snygg.exitStatus === 0) status = "terminated";
		else status = "crashed with exit code " + snygg.exitStatus;
		document.getElementById('statusField').textContent = status;
	});

	snygg.addEventListener('message', function (ev) {
		var msg = JSON.parse(ev.data);
		if (msg.what === "status") {
			if (msg.status === "running") {
				document.getElementById('statusField').innerHTML = 'by <a href="http://magnushoff.com/">Magnus Hoff</a>';
			}
		} else if (msg.what === "died") {
			db.localHighscores.post({
				"timestamp": (new Date()).toISOString(),
				"board": msg.board,
				"score": msg.score
			});
		} else if (msg.what === "score_updated") {
			document.getElementById("score").textContent = msg.score;
		}
	});

	function initializeFullscreen(keyCode, box) {
		function toggleFullscreen() {
			var isFullscreen =
				document.fullscreenEnabled ||
				document.fullScreenElement ||
				document.mozFullScreen ||
				document.webkitIsFullScreen;

			if (!isFullscreen) {
				var requestFullscreen =
					box.requestFullscreen ||
					box.mozRequestFullScreen ||
					box.webkitRequestFullscreen;
				requestFullscreen.call(box);
			} else {
				var exitFullscreen =
					document.exitFullscreen ||
					document.mozCancelFullScreen ||
					document.webkitExitFullscreen;
				exitFullscreen.call(document);
			}
		}

		box.addEventListener('keyup', function (ev) {
			if (ev.keyCode === keyCode) {
				ev.preventDefault();
				toggleFullscreen();
			}
		});
	}

	initializeFullscreen(70, document.getElementById("fullscreenbox"));
}

document.addEventListener("DOMContentLoaded", installApp);

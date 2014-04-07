require({
	waitSeconds: 0,
	shim: {
		"md5": { exports: "md5" },
		"sha1": { exports: "CryptoJS" }
	}
}, [
	'game',
	'login',
	'highscore-reporter',
	'gravatar-provider',
	'highscore-list',
	'board-selector',
	'tweeter',
	'fullscreen',
	'deps/pouchdb',
	'deps/domReady!'
], function(
	Game,
	Login,
	HighscoreReporter,
	GravatarProvider,
	HighscoreList,
	BoardSelector,
	Tweeter,
	initializeFullscreen,
	PouchDB
) {
	var centralCouch = 'https://mag.cloudant.com/';

	var highscoreReporter = new HighscoreReporter(centralCouch);

	var gravatarProvider = new GravatarProvider(centralCouch);
	var highscoreList = new HighscoreList(document.getElementById("highscore-list"), centralCouch, gravatarProvider);

	var login = new Login(document.getElementById("login-interaction"), centralCouch, highscoreReporter);

	var tweeter = new Tweeter(document.getElementById("tweeter"));

	var boardSelector = new BoardSelector(
		document.getElementById("board"),
		{
			boardChanged: function (board) {
				if (game) game.loadBoard(board);
				highscoreList.load(board);
			}
		}
	);

	var keyCodeForF = 70;
	initializeFullscreen(keyCodeForF, document.getElementById("fullscreenbox"));

	var game = new Game(
		document.getElementById("snygg"),
		{
			didLoad: function () {
				boardSelector.loadBoard();
			},
			status: function (status, html) {
				document.getElementById('statusField')[html ? "innerHTML" : "textContent"] = status;
			},
			died: function (board, score) {
				if (score > 0) {
					highscoreReporter.registerScore(board, score);
					if (login.userDoc) {
						var highscoreIndex = highscoreList.applyScore({
							name: login.userDoc.name,
							score: score,
							timestamp: (new Date()).toISOString()
						});
						if (highscoreIndex === 0) {
							tweeter.tweet({
								text: "With " + score + " point" + (score !== 1 ? "s" : "") + " " +
									"I just beat the high score!",
								url: window.location,
								via: "magistratic"
							});
						}
						if (highscoreIndex !== -1) highscoreList.render();
					}
				}
			},
			score: function (score) {
				document.getElementById("score").textContent = score;
			}
		}
	);
});

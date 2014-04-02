require({
	shim: {
		"md5": { exports: "md5" },
		"sha1": { exports: "CryptoJS" }
	}
}, [
	'game',
	'login',
	'highscore-reporter',
	'highscore-list',
	'board-selector',
	'fullscreen',
	'deps/pouchdb',
	'deps/domReady!'
], function(
	Game,
	Login,
	HighscoreReporter,
	HighscoreList,
	BoardSelector,
	initializeFullscreen,
	PouchDB
) {
	var centralCouch = 'https://mag.cloudant.com/';

	var highscoreReporter = new HighscoreReporter(centralCouch);

	var highscoreList = new HighscoreList(document.getElementById("highscore-list"), centralCouch);

	var login = new Login(document.getElementById("login-interaction"), centralCouch, highscoreReporter);

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
				if (score > 0) highscoreReporter.registerScore(board, score);
			},
			score: function (score) {
				document.getElementById("score").textContent = score;
			}
		}
	);
});

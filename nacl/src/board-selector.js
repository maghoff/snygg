define([], function () {
	function BoardSelector(dom, listener) {
		if (!this instanceof BoardSelector) return new BoardSelector(dom, listener);

		this.dom = dom;
		this.listener = listener;

		this.dom.addEventListener('change', this.selectionChanged.bind(this));
		window.addEventListener('hashchange', this.loadBoard.bind(this));
		this.loadBoard();
	}

	BoardSelector.prototype.selectionChanged = function () {
		var boardItem = this.dom.options[this.dom.selectedIndex];
		window.history.pushState(
			null,
			"Snygg: " + boardItem.name,
			"#" + boardItem.value
		);
		this.loadBoard();
	};

	BoardSelector.prototype.loadBoard = function () {
		var board = window.location.hash.substr(1);
		if (!this.dom.namedItem(board)) {
			if (board) {
				window.history.pushState(null, "Snygg: Pill", "#pill");
			}
			board = "pill";
		}
		this.dom.namedItem(board).setAttribute("selected", "selected");

		this.listener.boardChanged(board);
	}

	return BoardSelector;
});

define([], function () {
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

	return initializeFullscreen;
});

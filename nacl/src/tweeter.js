define([
	'encode-query-args'
], function (
	encodeQueryArgs
) {
	function Tweeter(dom) {
		if (!this instanceof Tweeter) return new Tweeter(dom);

	   this.dom = dom;
	}

	Tweeter.prototype.tweet = function (opts) {
		this.dom.getElementsByClassName("suggested-tweet")[0].textContent = opts.text;
		var url = "http://twitter.com/share?" + encodeQueryArgs(opts);
		this.dom.getElementsByClassName("tweet-this")[0].setAttribute("href", url);
		this.dom.classList.add("active");
	};

	return Tweeter;
});

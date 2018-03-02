var fs = require("fs");
var resource_tree = require("resource-tree");
var Mustache = require("mustache");

function MustacheResource(templateFile, view, contentType) {
	resource_tree.Resource.call(this);
	this.templateFile = templateFile;
	this.view = view;
	this.contentType = contentType || "text/html;charset=utf8";
}
MustacheResource.prototype = new resource_tree.Resource();
MustacheResource.constructor = MustacheResource;

MustacheResource.prototype.http_GET = function (req, res) {
	var self = this;

	fs.readFile(this.templateFile, "utf-8", function (err, template) {
		if (err) {
			console.log("[MustacheResource] Error:", err);
			res.writeHead(500, { "Content-Type": "text/plain", "Cache-Control": "no-cache" });
			res.end("500 Internal Server Error\n");
			return;
		}

		res.writeHead(200, { "Content-Type": self.contentType, "Cache-Control": "no-cache" });
		res.end(Mustache.render(template, self.view));
	});
};

module.exports = MustacheResource;

import buildTsc from "@izui/scripts/webpack/buildTsc";

buildTsc({
	packageJsonPath: require.resolve("../package.json"),
	outputPath: "build",
	copyTo: [
		{ from: "runtime" },
		{ from: "LICENSE" },
		{ from: "README.md" }
	]
});

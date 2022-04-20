import buildTsc from "@izui/scripts/webpack/buildTsc";

export default () => buildTsc({
	packageJsonPath: require.resolve("../package.json"),
	outputPath: "build",
	copyTo: [
		{ from: "runtime", to: "runtime" },
		{ from: "LICENSE" },
		{ from: "README.md" },
		{ from: "yarn.lock" },
		{ from: ".npmignore" }
	]
});

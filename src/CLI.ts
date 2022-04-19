import yargs from "yargs";
import { hideBin } from "yargs/helpers";
import { exit } from "process";
import { execa } from "execa";
import fs from "fs/promises";
import path from "path";
import ini from "ini";
import { fileURLToPath } from "url";

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

/**
 * Install the default CoD4 runtime if needed.
 * @param location - The CoD4 location.
 */
const installRuntime = async (location: string): Promise<void> =>
	fs.cp(path.join(__dirname, "../runtime"), location, {
		recursive: true,
		force: false
	});

/**
 * Run the CoD4 binary.
 * @param target - The CoD4 binary name.
 * @param location - The CoD4 directory path.
 * @param args - The CoD4 arguments.
 * @returns
 */
const run = async (target: string, location: string, args: string[]): Promise<number> =>
{
	args.push("+map mp_gscunit");
	const command = path.join(location, target);
	const proc = await execa(command, args, {
		cwd: location,
		stderr: "inherit",
		windowsVerbatimArguments: true
	});
	return proc.exitCode;
};

/**
 * Read a INI profile.
 * @param path - The INI filepath.
 * @returns
 */
const readProfile = async (path: string): Promise<string[]> =>
{
	await fs.access(path);
	const config = ini.parse(await fs.readFile(path, "utf-8"));

	return Object.values<Record<string, string>>(config)
		.flatMap(section => Object.entries(section))
		.map(([key, value]) => `--${key}=${value}`);
};

/**
 * Parse the CLI arguments.
 * @param args - The CLI arguments.
 * @returns
 */
const parseArgs = (args: string[]) => yargs(args).options({
	location: 	{ alias: "l", describe: "The CoD4 path location", type: "string" },
	target: 	{ alias: "t", describe: "The CoD4 binary target name", type: "string", default: "cod4x18_dedrun" },
	args: 		{ alias: "a", describe: "The CoD4 arguments", type: "string" },
	profile: 	{ alias: "p", describe: "The gscunit profile path", type: "string" }
}).help();

/**
 * gscunit entry point.
 */
const main = async (): Promise<void> =>
{
	console.log("A unit testing framework for GSC\n");
	let options = parseArgs(hideBin(process.argv));

	if (options.argv.profile)
		options = parseArgs(await readProfile(options.argv.profile));
	if (!options.argv.location)
	{
		options.showHelp();
		exit(0);
	}
	const { argv: { location, target, args = "" } } = options;

	await installRuntime(location);
	exit(await run(target, location, [args]));
};

main();
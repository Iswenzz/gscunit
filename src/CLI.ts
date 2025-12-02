#!/usr/bin/env node
import fs from "fs/promises";
import yargs from "yargs";
import path from "path";
import ini from "ini";

import { hideBin } from "yargs/helpers";
import { exit } from "process";
import { execa } from "execa";
import { fileURLToPath } from "url";

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const installRuntime = async (location: string): Promise<void> =>
	fs.cp(path.join(__dirname, "../runtime"), location, {
		recursive: true,
		force: false
	});

const run = async (target: string, location: string, args: string[]): Promise<number> => {
	args.push("+map_rotate", "+set gscunit 1");
	const command = path.join(location, target);
	const proc = await execa(command, args, {
		cwd: location,
		stdin: "ignore",
		stdout: "ignore",
		stderr: "inherit",
		windowsVerbatimArguments: true
	});
	return proc.exitCode || 0;
};

const readProfile = async (path: string): Promise<string[]> => {
	await fs.access(path);
	const config = ini.parse(await fs.readFile(path, "utf-8"));

	return Object.values<Record<string, string>>(config)
		.flatMap(section => Object.entries(section))
		.map(([key, value]) => `--${key}=${value}`);
};

const parseArgs = (args: string[]) =>
	yargs(args)
		.options({
			location: {
				alias: "l",
				describe: "The CoD4 path location",
				type: "string"
			},
			target: {
				alias: "t",
				describe: "The CoD4 binary target name",
				type: "string",
				default: "cod4x18_dedrun"
			},
			args: {
				alias: "a",
				describe: "The CoD4 arguments",
				type: "string"
			},
			profile: {
				alias: "p",
				describe: "The gscunit profile path",
				type: "string"
			}
		})
		.check(argv => {
			if (!argv.location && !argv.profile) {
				throw new Error("You must provide either --location or --profile.");
			}
			if (argv.location && argv.profile) {
				throw new Error("You must provide either --location or --profile.");
			}
			return true;
		})
		.help()
		.parse();

const main = async (): Promise<void> => {
	console.log("A unit testing framework for GSC\n");
	let options = await parseArgs(hideBin(process.argv));
	if (options.profile) options = await parseArgs(await readProfile(options.profile));
	const { location = "", target = "", args = "" } = options;
	await installRuntime(location);
	exit(await run(target, location, [args]));
};

main();

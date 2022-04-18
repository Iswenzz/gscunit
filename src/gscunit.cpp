#include <cxxopts.hpp>
#include <cmrc/cmrc.hpp>
#include <boost/process.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <filesystem>

#ifdef _WIN32
	#include <boost/process/windows.hpp> 
#endif

CMRC_DECLARE(runtime);

/// <summary>
/// Retrieve resources from CMRC.
/// </summary>
/// <param name="fs">The CMRC file system.</param>
/// <param name="name">The resource name.</param>
/// <param name="fullpath">The resource build path.</param>
/// <returns></returns>
std::map<std::string, cmrc::file> resource(const cmrc::embedded_filesystem& fs, 
	const std::string& name, const std::string& fullpath = "")
{
	std::map<std::string, cmrc::file> files{ };
	auto it = fs.iterate_directory(fullpath.empty() ? name : name + "/" + fullpath);

	std::vector<cmrc::directory_entry> entries(it.begin(), it.end());
	for (cmrc::directory_entry& entry : entries)
	{
		std::string filepath = fullpath.empty() ? entry.filename() : (fullpath + "/" + entry.filename());
		std::string resourcePath = name + "/" + filepath;

		if (entry.is_directory())
		{
			std::map<std::string, cmrc::file> dirFiles = resource(fs, name, filepath);
			files.insert(dirFiles.cbegin(), dirFiles.cend());
		}
		else if (entry.is_file())
			files.emplace(filepath, fs.open(resourcePath));
	}
	return files;
}

/// <summary>
/// Install the default CoD4 runtime to run gscunit if needed.
/// </summary>
/// <param name="cod4Location">The CoD4 directory path.</param>
void installRuntime(const std::string& cod4Location)
{
	cmrc::embedded_filesystem fs = cmrc::runtime::get_filesystem();
	std::map<std::string, cmrc::file> runtime = resource(fs, "runtime");

	for (auto& [name, file] : runtime)
	{
		std::ofstream stream;
		std::filesystem::path path = std::filesystem::path(cod4Location) / name;

		if (std::filesystem::exists(path))
			continue;

		stream.open(std::filesystem::path(cod4Location) / name, std::ios::binary | std::ios::out);
		stream.write(file.cbegin(), file.size());
		stream.close();
	}
}

/// <summary>
/// Run the CoD4 binary.
/// </summary>
/// <param name="cod4Target">The CoD4 binary name.</param>
/// <param name="cod4Location">The CoD4 directory path.</param>
/// <param name="cod4Args">The CoD4 arguments.</param>
/// <returns></returns>
int run(const std::string& cod4Target, const std::string& cod4Location, std::string& cod4Args)
{
	std::cout << "Global test environment set-up." << std::endl;

	std::string target = (std::filesystem::path(cod4Location) / cod4Target).string();
#ifdef _WIN32
	target += ".exe";
#endif
	cod4Args += " +map mp_gscunit";

	boost::process::child child(
		boost::process::exe(boost::filesystem::path(target)),
		boost::process::args(cod4Args),
		boost::process::std_in.close(),
		boost::process::std_out.close(),
		boost::process::std_err > stderr,
		boost::process::start_dir(cod4Location)
#ifdef _WIN32
		,boost::process::windows::hide
#endif
	);
	child.wait();
	return child.exit_code();
}

/// <summary>
/// Read a INI profile.
/// </summary>
/// <param name="path">The INI filepath.</param>
/// <returns></returns>
std::vector<std::string> readProfile(const std::string& path)
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(path, pt);

	std::vector<std::string> settings;
	settings.push_back("--cod4-location=" + pt.get<std::string>("CLI.cod4-location"));
	settings.push_back("--args=" + pt.get<std::string>("CLI.args"));
	return settings;
}

/// <summary>
/// gscunit entry point.
/// </summary>
/// <param name="argc">The arg count.</param>
/// <param name="argv">The arg values.</param>
/// <returns></returns>
int main(int argc, char** argv)
{
	cxxopts::Options options("gscunit", "A unit testing framework for GSC.");
	options.add_options()
		("l,cod4-location", "The CoD4 path location", cxxopts::value<std::string>())
		("t,target", "The CoD4 binary target name", cxxopts::value<std::string>()->default_value("cod4x18_dedrun"))
		("a,args", "The CoD4 arguments", cxxopts::value<std::string>())
		("p,profile", "The gscunit profile path", cxxopts::value<std::string>())
		("h,help", "Print usage");
	cxxopts::ParseResult result = options.parse(argc, argv);

	if (result.count("profile"))
	{
		std::vector<std::string> profile = readProfile(result["profile"].as<std::string>());
		std::vector<const char*> values;

		values.push_back(argv[0]);
		for (const std::string& value : profile)
			values.push_back(value.c_str());
		result = options.parse(values.size(), values.data());
	}
    if (result.count("help") || !result.count("cod4-location"))
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }
	std::string cod4Location = result["cod4-location"].as<std::string>();
	std::string cod4Target = result["target"].as<std::string>();
	std::string cod4Args = result.count("args") ? result["args"].as<std::string>() : "";

	installRuntime(cod4Location);
	return run(cod4Target, cod4Location, cod4Args);
}

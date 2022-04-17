#include <iostream>
#include <fstream>
#include <cxxopts.hpp>
#include <cmrc/cmrc.hpp>
#include <filesystem>

CMRC_DECLARE(runtime);

std::map<std::string, cmrc::file> resource(const cmrc::embedded_filesystem& fs, 
	std::string name, std::string fullpath = "")
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

void installRuntime(std::string cod4Location)
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

int run(std::string cod4Location, std::string cod4Args)
{
	return 0;
}

int main(int argc, char** argv)
{
	cxxopts::Options options("gscunit", "description");
	options.add_options()
		("cod4-location", "The CoD4 path location", cxxopts::value<std::string>())
		("args", "The CoD4 arguments", cxxopts::value<std::string>())
		("h,help", "Print usage");
	cxxopts::ParseResult result = options.parse(argc, argv);

    if (result.count("help") || !result.count("cod4-location"))
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }
	std::string cod4Location = result["cod4-location"].as<std::string>();
	std::string cod4Args = result.count("args") ? cod4Args = result["args"].as<std::string>() : "";

	installRuntime(cod4Location);
	return run(cod4Location, cod4Args);
}


#include "config.h"

#include <string>
#include <vector>
#include <cassert>

#include <yaml-cpp/yaml.h>

namespace elect {

  struct YamlElect {
    int max_lease_timeout_;
    int acquire_lease_timeout_;
    std::vector<std::string> node_addrs_;
  };

}

namespace YAML {
  template <>
  struct convert<elect::YamlElect> {
    static Node encode(const elect::YamlElect &rhs) {
      YAML::Node node;
      node["max_lease_timeout"] = rhs.max_lease_timeout_;
      node["acquire_lease_timeout"] = rhs.acquire_lease_timeout_;
      node["address"] = rhs.node_addrs_;

      return node;
    }

    static bool decode(const Node &node, elect::YamlElect &rhs) {
      if (!node.IsMap() || node.size() != 3)
        return false;
      rhs.max_lease_timeout_ = node["max_lease_timeout"].as<int>();
      rhs.acquire_lease_timeout_ = node["acquire_lease_timeout"].as<int>();
      rhs.node_addrs_ = node["address"].as<std::vector<std::string>>();

      return true;
    }
  };

}

namespace elect {

  Config &Config::GetInstance() {
    static Config config(std::move(GetData()));
    return config;
  }

  Data Config::GetData() {
    auto config = YAML::LoadFile("/path/to/file");
    assert(config["elect"]);
    auto load_data = config["elect"].as<elect::YamlElect>();

    Data data{};
    data.max_lease_timeout = load_data.max_lease_timeout_;
    data.acquire_lease_timeout = load_data.acquire_lease_timeout_;
    for (const auto &var : load_data.node_addrs_) {
      auto pos = var.find(':');
      auto ip = var.substr(0, pos);
      auto port = ::strtol(var.c_str() + pos + 1, nullptr, 10);
      data.node_addrs.emplace_back(ip, port);
    }

    return data;
  }
}
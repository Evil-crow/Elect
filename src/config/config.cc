
#include "config.h"

#include <string>
#include <vector>
#include <cassert>

#include <yaml-cpp/yaml.h>

namespace elect {

  struct YamlElect {
    int node_no_;
    std::string node_id_;
    std::string addr_;
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
      node["No"] = rhs.node_no_;
      node["node"] = rhs.node_id_;
      node["addr"] = rhs.addr_;
      node["max_lease_timeout_"] = rhs.max_lease_timeout_;
      node["acquire_lease_timeout"] = rhs.acquire_lease_timeout_;
      node["address"] = rhs.node_addrs_;

      return node;
    }

    static bool decode(const Node &node, elect::YamlElect &rhs) {
      if (!node.IsMap() || node.size() != 6)
        return false;
      rhs.node_no_ = node["No"].as<int>();
      rhs.node_id_ = node["node"].as<std::string>();
      rhs.addr_ = node["addr"].as<std::string>();
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
    auto config = YAML::LoadFile("./elect.yaml");
    assert(config["elect"]);
    auto load_data = config["elect"].as<elect::YamlElect>();

    Data data{};
    data.node_no_ = load_data.node_no_;
    data.max_lease_timeout_ = load_data.max_lease_timeout_;
    data.acquire_lease_timeout = load_data.acquire_lease_timeout_;
    data.node_id_ = load_data.node_id_;
    data.addr_ = load_data.addr_;
    for (const auto &var : load_data.node_addrs_) {
      auto pos = var.find(':');
      auto ip = var.substr(0, pos);
      auto port = ::strtol(var.c_str() + pos + 1, nullptr, 10);
      data.node_addrs_.emplace_back(ip, port);
    }
    auto pos = load_data.node_id_.find(':');
    data.ip_ = load_data.node_id_.substr(0, pos);
    data.port_ = ::strtol(load_data.node_id_.c_str() + pos + 1, nullptr, 10);

    return data;
  }
}
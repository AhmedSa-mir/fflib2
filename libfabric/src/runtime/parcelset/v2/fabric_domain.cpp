//  Copyright (c) 2018 Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/runtime/parcelset/v2/fabric_domain.hpp>
#include <hpx/runtime/parcelset/v2/fabric_status.hpp>
#include <hpx/util/assert.hpp>

#include <rdma/fabric.h>
#include <rdma/fi_domain.h>

#include <netinet/in.h>

#include <cstring>
#include <string>
#include <utility>
#include <vector>

namespace hpx { namespace parcelset { inline namespace v2 {

    fabric_provider get_fabric_provider(fi_info* info)
    {
        std::string provider(info->fabric_attr->prov_name);

        if (provider == "bgq")
        {
            return fabric_provider::bgq;
        }
        if (provider == "gni")
        {
            return fabric_provider::gni;
        }
        if (provider == "mlx")
        {
            return fabric_provider::mlx;
        }
        if (provider == "netdir")
        {
            return fabric_provider::netdir;
        }
        if (provider == "psm")
        {
            return fabric_provider::psm;
        }
        if (provider == "psm2")
        {
            return fabric_provider::psm2;
        }
        if (provider == "sockets")
        {
            return fabric_provider::sockets;
        }
        if (provider == "shm")
        {
            return fabric_provider::shm;
        }
        if (provider == "udp")
        {
            return fabric_provider::udp;
        }
        if (provider == "usnic")
        {
            return fabric_provider::usnic;
        }
        if (provider == "verbs")
        {
            return fabric_provider::verbs;
        }

        HPX_ASSERT(false);

        return fabric_provider::num;
    }

    const char* fabric_provider_name(fabric_provider prov)
    {
        switch (prov)
        {
            case fabric_provider::bgq:
            {
                return "bgq";
            }
            case fabric_provider::gni:
            {
                return "gni";
            }
            case fabric_provider::mlx:
            {
                return "mlx";
            }
            case fabric_provider::netdir:
            {
                return "netdir";
            }
            case fabric_provider::psm:
            {
                return "psm";
            }
            case fabric_provider::psm2:
            {
                return "psm2";
            }
            case fabric_provider::sockets:
            {
                return "sockets";
            }
            case fabric_provider::shm:
            {
                return "shm";
            }
            case fabric_provider::udp:
            {
                return "udp";
            }
            case fabric_provider::usnic:
            {
                return "usnic";
            }
            case fabric_provider::verbs:
            {
                return "verbs";
            }
            default:
                return "unkown";
        }
    }

    std::ostream& operator<<(std::ostream& os, fabric_provider prov)
    {
        os << fabric_provider_name(prov);
        return os;
    }

}}}

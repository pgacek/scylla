/*
 * Copyright 2019 ScyllaDB
 */

/*
 * This file is part of Scylla.
 *
 * Scylla is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Scylla is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Scylla.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <string>
#include <string_view>
#include "types.hh"
#include "schema.hh"
#include "keys.hh"
#include "rjson.hh"
#include "utils/big_decimal.hh"

namespace alternator {

enum class alternator_type : int8_t {
    S, B, BOOL, N, NOT_SUPPORTED_YET
};

struct type_info {
    alternator_type atype;
    data_type dtype;
};

struct type_representation {
    std::string ident;
    data_type dtype;
};

type_info type_info_from_string(std::string type);
type_representation represent_type(alternator_type atype);

bytes serialize_item(const rjson::value& item);
rjson::value deserialize_item(bytes_view bv);

std::string type_to_string(data_type type);

bytes get_key_column_value(const rjson::value& item, const column_definition& column);
bytes get_key_from_typed_value(const rjson::value& key_typed_value, const column_definition& column);
rjson::value json_key_column_value(bytes_view cell, const column_definition& column);

partition_key pk_from_json(const rjson::value& item, schema_ptr schema);
clustering_key ck_from_json(const rjson::value& item, schema_ptr schema);

// If v encodes a number (i.e., it is a {"N": [...]}, returns an object representing it.  Otherwise,
// raises ValidationException with diagnostic.
big_decimal unwrap_number(const rjson::value& v, std::string_view diagnostic);

// Check if a given JSON object encodes a set (i.e., it is a {"SS": [...]}, or "NS", "BS"
// and returns set's type and a pointer to that set. If the object does not encode a set,
// returned value is {"", nullptr}
const std::pair<std::string, const rjson::value*> unwrap_set(const rjson::value& v);

}

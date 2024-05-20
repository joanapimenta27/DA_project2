//
// Created by joana on 15-05-2024.
//

#include "Haversine.h"
#include <cmath>


/**
 * @brief Calculate the great-circle distance between two points on the Earth
 *        specified by their latitude and longitude using the Haversine formula.
 *
 * This function computes the shortest distance over the Earth's surface,
 * giving an "as-the-crow-flies" distance between the points (ignoring any hills,
 * valleys, or other obstacles).
 *
 * @param latitude1 Latitude of the first point in degrees.
 * @param longitude1 Longitude of the first point in degrees.
 * @param latitude2 Latitude of the second point in degrees.
 * @param longitude2 Longitude of the second point in degrees.
 * @return The distance between the two points in kilometers.
 *
 * @complexity Constant time complexity O(1), as the number of operations
 * does not depend on the input size.
 */
auto calculate_distance(const angle_t latitude1,
                        const angle_t longitude1,
                        const angle_t latitude2,
                        const angle_t longitude2) -> kilometers_t
{
    const auto earths_radius = kilometers_t{6371};

    // Get the difference between our two points then convert the difference into radians
    const auto lat_delta = convert(latitude2 - latitude1);
    const auto lon_delta = convert(longitude2 - longitude1);

    const auto converted_lat1 = convert(latitude1);
    const auto converted_lat2 = convert(latitude2);

    const auto a =
        pow(sin(lat_delta / 2), 2) + cos(converted_lat1) * cos(converted_lat2) * pow(sin(lon_delta / 2), 2);

    const auto c = 2 * atan2(sqrt(a), sqrt(1 - a));
    const auto d = earths_radius * c;

    return d;
}

// convert our passed value to radians_t
auto convert(const angle_t angle) -> radians_t
{
    return angle * (M_PI / 180);
}
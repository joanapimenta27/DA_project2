//
// Created by joana on 15-05-2024.
//

#ifndef HAVERSINE_H
#define HAVERSINE_H



using angle_t      = double;
using radians_t    = double;
using kilometers_t = double;

auto calculate_distance(const angle_t latitude1,
                        const angle_t longitude1,
                        const angle_t latitude2,
                        const angle_t longitude2) -> kilometers_t;

auto convert(const angle_t angle) -> radians_t;





#endif //HAVERSINE_H

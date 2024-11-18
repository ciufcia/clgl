#pragma once

#include <string>

namespace clgl::color_mappings_creator
{

const std::string output_path = "color_mappings.clcm";

Color template_palette[16] {

/*0*/  { 0u, 0u, 0u },

/*1*/  { 0u, 0u, 128u },

/*2*/  { 0u, 128u, 0u },

/*3*/  { 0u, 128u, 128u },

/*4*/  { 128u, 0u, 0u },

/*5*/  { 128u, 0u, 128u },

/*6*/  { 128u, 128u, 0u },

/*7*/  { 192u, 192u, 192u },

/*8*/  { 128u, 128u, 128u },

/*9*/  { 0u, 0u, 255u },

/*10*/ { 0u, 255u, 0u },

/*11*/ { 0u, 255u, 255u },

/*12*/ { 255u, 0u, 0u },

/*13*/ { 255u, 0u, 255u },

/*14*/ { 255u, 255u, 0u },

/*15*/ { 255u, 255u, 255u }

};

}
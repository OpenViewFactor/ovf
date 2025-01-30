# OpenViewFactor

## Purpose

OpenViewFactor is a portable hardware-accelerated solver for radiation view factors. This utility is designed to enable the evaluation of view factors between any two geometries. OpenViewFactor is intended to be used for real-world systems, so it supports evaluation of view factors in the presence of obstructing geometries, as well.

## Approach

This is done with an object-oriented C++ implementation with an easy-to-use command-line interface. This solver is accelerated via the OpenMP api for multi-core processing.

## Future Development

Eventually, this solver will be GPU-accelerated as well using the CUDA interface for select Nvidia GPU cards. This decision was made to best support the de facto standard for GPU programming, as CUDA is the most robust interface for such applications. In the far future, I may investigate more generalized GPU acceleration interfaces to support non-CUDA Nvidia and AMD cards.

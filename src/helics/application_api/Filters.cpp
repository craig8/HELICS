/*

Copyright (C) 2017, Battelle Memorial Institute
All rights reserved.

This software was co-developed by Pacific Northwest National Laboratory, operated by the Battelle Memorial
Institute; the National Renewable Energy Laboratory, operated by the Alliance for Sustainable Energy, LLC; and the
Lawrence Livermore National Laboratory, operated by Lawrence Livermore National Security, LLC.

*/

#include "Filters.hpp"
#include "MessageOperators.h"

#include <memory>
#include <random>
#include <thread>

namespace helics
{
static void addOperations (Filter *filt, defined_filter_types type)
{
    switch (type)
    {
    case custom:
        break;
    case randomDelay:
    {
        auto op = std::make_shared<randomDelayFilterOperation> ();
        filt->setFilterOperations (std::move (op));
    }
    break;
    case delay:
    {
        auto op = std::make_shared<delayFilterOperation> ();
        filt->setFilterOperations (std::move (op));
    }
    break;
    case randomDrop:
    {
        auto op = std::make_shared<randomDropFilterOperation> ();
        filt->setFilterOperations (std::move (op));
    }
    break;
    }
}

std::unique_ptr<DestinationFilter> make_destination_filter (defined_filter_types type,
                                                            MessageFilterFederate *mFed,
                                                            const std::string &target,
                                                            const std::string &name)

{
    auto dfilt = std::make_unique<DestinationFilter> (mFed, target, name);
    addOperations (dfilt.get (), type);
    return dfilt;
}

std::unique_ptr<SourceFilter> make_Source_filter (defined_filter_types type,
                                                  MessageFilterFederate *mFed,
                                                  const std::string &target,
                                                  const std::string &name)
{
    auto sfilt = std::make_unique<SourceFilter> (mFed, target, name);
    addOperations (sfilt.get (), type);
    return sfilt;
}

void FilterOperations::set (const std::string & /*property*/, double /*val*/) {}
void FilterOperations::setString (const std::string & /*property*/, const std::string & /*val*/) {}

delayFilterOperation::delayFilterOperation (Time delayTime) : delay (delayTime)
{
    if (delayTime < timeZero)
    {
        delay = timeZero;
    }
    td = std::make_shared<MessageTimeOperator> ([this](Time messageTime) { return messageTime + delay; });
}

void delayFilterOperation::set (const std::string &property, double val)
{
    if (property == "delay")
    {
        if (val >= timeZero)
        {
            delay = Time (val);
        }
    }
}

std::shared_ptr<MessageOperator> delayFilterOperation::getOperator ()
{
    return std::static_pointer_cast<MessageOperator> (td);
}

// enumeration of possible random number generator distributions
enum class random_dists_t : int
{
	constant,
    uniform,
    bernoulli,
    binomial,
    geometric,
    poisson,
    exponential,
    gamma,
    weibull,
    extreme_value,
    normal,
    lognormal,
    chi_squared,
    cauchy,
    fisher_f,
    student_t
};

double randDouble (random_dists_t dist, double p1, double p2)
{
#ifndef __apple_build_version__
    static thread_local std::mt19937 generator (
      std::random_device{}() +
      static_cast<unsigned int> (std::hash<std::thread::id>{}(std::this_thread::get_id ())));
#else
#if __clang_major__ >= 8
    static thread_local std::mt19937 generator (
      std::random_device{}() +
      static_cast<unsigned int> (std::hash<std::thread::id>{}(std::this_thread::get_id ())));
#else
    static __thread std::mt19937 *genPtr =
      new std::mt19937 (std::random_device{}() +
                        static_cast<unsigned int> (std::hash<std::thread::id>{}(std::this_thread::get_id ())));
    auto &generator = *genPtr;
#endif
#endif
    switch (dist)
    {
	case random_dists_t::constant:
		return p1;
    case random_dists_t::uniform:
    default:
    {
        std::uniform_real_distribution<double> distribution (p1, p2);
        return distribution (generator);
    }
	break;
	case random_dists_t::normal:
	{
		std::normal_distribution<double> distribution(p1, p2);
		return distribution(generator);
	}
	break;
	case random_dists_t::lognormal:
	{
		std::lognormal_distribution<double> distribution(p1, p2);
		return distribution(generator);
	}
	break;
	case random_dists_t::cauchy:
	{
		std::cauchy_distribution<double> distribution(p1, p2);
		return distribution(generator);
	}
	break;
    }

   // return 0.0;
}

class randomDelayGenerator
{
  public:
    Time delay;
};

randomDelayFilterOperation::randomDelayFilterOperation () {}
randomDelayFilterOperation::~randomDelayFilterOperation () {}
void randomDelayFilterOperation::set (const std::string &property, double val) {}
void randomDelayFilterOperation::setString (const std::string &property, const std::string &val) {}

std::shared_ptr<MessageOperator> randomDelayFilterOperation::getOperator ()
{
    return std::static_pointer_cast<MessageOperator> (td);
}

randomDropFilterOperation::randomDropFilterOperation ()
{
    tcond = std::make_shared<MessageConditionalOperator> (
      [this](const Message *) { return (randDouble (random_dists_t::uniform, 0, 1.0) < dropProb); });
}

randomDropFilterOperation::~randomDropFilterOperation () {}
void randomDropFilterOperation::set (const std::string &property, double val) {}
void randomDropFilterOperation::setString (const std::string &property, const std::string &val) {}

std::shared_ptr<MessageOperator> randomDropFilterOperation::getOperator ()
{
    return std::static_pointer_cast<MessageOperator> (tcond);
}
}  // namespace helics
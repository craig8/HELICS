/*
Copyright © 2017-2018,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC
All rights reserved. See LICENSE file and DISCLAIMER for more details.
*/
#pragma once

#include "HelicsPrimaryTypes.hpp"
#include "ValueFederate.hpp"
#include "helicsTypes.hpp"
#include <algorithm>
#include <array>
namespace helics
{
/** base class for a input object*/
class Input
{
  protected:
    ValueFederate *fed = nullptr;  //!< reference to the value federate
    interface_handle handle;  //!< the id as generated by the Federate
  private:
    int referenceIndex = -1;  //!< an index used for callback lookup
    void *dataReference = nullptr;  //!< pointer to a piece of containing data

    helics_type_t type = helics_type_t::helicsInvalid;  //!< the underlying type the publication is using
    bool changeDetectionEnabled = false;  //!< the change detection is enabled
    bool hasUpdate = false;  //!< the value has been updated
    defV lastValue;  //!< the last value updated
    double delta = -1.0;  //!< the minimum difference
    std::string actualName;  //!< the name of the federate
    // this needs to match the defV type
    mpark::variant<std::function<void(const double &, Time)>,
                   std::function<void(const int64_t &, Time)>,
                   std::function<void(const std::string &, Time)>,
                   std::function<void(const std::complex<double> &, Time)>,
                   std::function<void(const std::vector<double> &, Time)>,
                   std::function<void(const std::vector<std::complex<double>> &, Time)>,
                   std::function<void(const named_point &, Time)>,
                   std::function<void(const bool &, Time)>>
      value_callback;  //!< callback function for the federate
  public:
    Input () = default;

    Input (ValueFederate *valueFed, interface_handle id, const std::string &actName)
        : fed (valueFed), handle (id), actualName (actName)
    {
    }

    Input (ValueFederate *valueFed,
           const std::string &name,
           const std::string &defaultType = "def",
           const std::string &units = std::string ());

    template <class FedPtr>
    Input (FedPtr &valueFed,
           const std::string &name,
           const std::string &defaultType = "def",
           const std::string &units = std::string ())
        : Input (std::addressof (*valueFed), name, defaultType, units)
    {
        static_assert (std::is_base_of<ValueFederate, std::remove_reference_t<decltype (*valueFed)>>::value,
                       "first argument must be a pointer to a ValueFederate");
    }

    Input (interface_visibility locality,
           ValueFederate *valueFed,
           const std::string &name,
           const std::string &defaultType = "def",
           const std::string &units = std::string ());

    template <class FedPtr>
    Input (interface_visibility locality,
           FedPtr &valueFed,
           const std::string &name,
           const std::string &defaultType = "def",
           const std::string &units = std::string ())
        : Input (locality, std::addressof (*valueFed), name, defaultType, units)
    {
        static_assert (std::is_base_of<ValueFederate, std::remove_reference_t<decltype (*valueFed)>>::value,
                       "first argument must be a pointer to a ValueFederate");
    }

    Input (ValueFederate *valueFed,
           const std::string &name,
           helics_type_t defType,
           const std::string &units = std::string ())
        : Input (valueFed, name, typeNameStringRef (defType), units)
    {
    }

    template <class FedPtr>
    Input (FedPtr &valueFed,
           const std::string &name,
           helics_type_t defType,
           const std::string &units = std::string ())
        : Input (valueFed, name, typeNameStringRef (defType), units)
    {
    }

    Input (interface_visibility locality,
           ValueFederate *valueFed,
           const std::string &name,
           const std::string &units = std::string ())
        : Input (locality, valueFed, name, "def", units)
    {
    }

    template <class FedPtr>
    Input (interface_visibility locality,
           FedPtr &valueFed,
           const std::string &key,
           const std::string &units = std::string ())
        : Input (locality, valueFed, key, "def", units)
    {
    }

    Input (interface_visibility locality,
           ValueFederate *valueFed,
           const std::string &name,
           helics_type_t defType,
           const std::string &units = std::string ())
        : Input (locality, valueFed, name, typeNameStringRef (defType), units)
    {
    }

    template <class FedPtr>
    Input (interface_visibility locality,
           FedPtr &valueFed,
           const std::string &name,
           helics_type_t defType,
           const std::string &units = std::string ())
        : Input (locality, valueFed, name, typeNameStringRef (defType), units)
    {
    }

    /** get the time of the last update
    @return the time of the last update
    */
    Time getLastUpdate () const { return fed->getLastUpdateTime (*this); }
    /** get the underlying handle that can be used to make direct calls to the Core API
     */
    interface_handle getHandle () const { return handle; }

    /** check if the Publication links to a valid operation*/
    bool isValid () const { return handle.isValid (); }
    bool operator< (const Input &inp) const { return (handle < inp.handle); }
    /** register a callback for an update notification
    @details the callback is called in the just before the time request function returns
    @param[in] callback a function with signature void( Time time)
    time is the time the value was updated  This callback is a notification callback and doesn't return the value
    */
    void registerNotificationCallback (std::function<void(Time)> callback)
    {
        fed->setInputNotificationCallback (*this, [this, callback](const Input &, Time time) {
            if (isUpdated ())
            {
                callback (time);
            }
        });
    }
    /** get the Name for the subscription
    @details the name is the local name if given, key is the full key name*/
    const std::string &getName () const { return actualName; }
    /** get the type of the input*/
    const std::string &getType () const { return fed->getInputType (*this); }
    /** get the units associated with a input*/
    const std::string &getUnits () const { return fed->getInputUnits (*this); }
    /** get an associated target*/
    const std::string &getTarget () const { return fed->getTarget (*this); }
    void addTarget (const std::string &newTarget) { fed->addTarget (*this, newTarget); }

    /** check if the value has been updated
	@details if changeDetection is Enabled this function also loads the value into the buffer*/
    bool isUpdated ();

    /** register a callback for the update
    @details the callback is called in the just before the time request function returns
    @param[in] callback a function with signature void(X val, Time time)
    val is the new value and time is the time the value was updated
    */
    template <class X>
    void setInputNotificationCallback (std::function<void(const X &, Time)> callback)
    {
        static_assert (helicsType<X> () != helics_type_t::helicsInvalid,
                       "callback type must be a primary helics type one of \"double, int64_t, named_point, bool, "
                       "std::vector<double>, std::vector<std::complex<double>>, std::complex<double>\"");
        value_callback = callback;
        fed->setInputNotificationCallback (*this, [this](Input &, Time time) { handleCallback (time); });
    }

    /** set the default value to use before any update has been published
     */
    template <class X>
    void setDefault (X &&val)
    {
        lastValue = make_valid (std::forward<X> (val));
    }

    /** set the minimum delta for change detection
    @param detltaV a double with the change in a value in order to register a different value
    */
    void setMinimumChange (double deltaV)
    {
        // this first check enables change detection if it was disabled via negative delta
        if (delta < 0.0)
        {
            changeDetectionEnabled = true;
        }
        delta = deltaV;
        // the second checks if we should disable from negative delta
        if (delta < 0.0)
        {
            changeDetectionEnabled = false;
        }
    }
    /** enable change detection
    @param enabled (optional) set to false to disable change detection true(default) to enable it
    */
    void enableChangeDetection (bool enabled = true) { changeDetectionEnabled = enabled; }

  private:
    /** deal with the callback from the application API*/
    void handleCallback (Time time);
    template <class X>
    void getValue_impl (std::true_type /*V*/, X &out)
    {
        if (fed->isUpdated (*this) || (hasUpdate && !changeDetectionEnabled))
        {
            auto dv = fed->getValueRaw (*this);
            if (type == helics_type_t::helicsInvalid)
            {
                type = getTypeFromString (fed->getPublicationType (*this));
            }
            if (type != helics_type_t::helicsInvalid)
            {
                valueExtract (dv, type, out);
                if (changeDetectionEnabled)
                {
                    if (changeDetected (lastValue, out, delta))
                    {
                        lastValue = make_valid (out);
                    }
                    else
                    {
                        valueExtract (lastValue, out);
                    }
                }
                else
                {
                    lastValue = make_valid (out);
                }
            }
            else
            {
                out = invalidValue<X> ();
            }
        }
        else
        {
            valueExtract (lastValue, out);
        }
        hasUpdate = false;
    }
    template <class X>
    void getValue_impl (std::false_type /*V*/, X &out)
    {
        std::conditional_t<std::is_integral<X>::value, int64_t, double> gval;
        getValue_impl (std::true_type (), gval);
        out = static_cast<X> (gval);
    }
    template <class X>
    X getValue_impl (std::true_type /*V*/)
    {
        X val;
        getValue_impl (std::true_type (), val);
        return val;
    }
    template <class X>
    X getValue_impl (std::false_type /*V*/)
    {
        std::conditional_t<std::is_integral<X>::value, int64_t, double> gval;
        getValue_impl (std::true_type (), gval);
        return static_cast<X> (gval);
    }

  public:
    int getValue (double *data, int maxsize);
    int getValue (char *str, int maxsize);
    /** get the latest value for the subscription
    @param[out] out the location to store the value
    */
    template <class X>
    void getValue (X &out)
    {
        static_assert (((helicsType<X> () != helics_type_t::helicsInvalid) || (isConvertableType<X> ())),
                       "requested types must be one of the primary helics types or convertible to one");
        getValue_impl<X> (std::conditional_t<(helicsType<X> () != helics_type_t::helicsInvalid), std::true_type,
                                             std::false_type> (),
                          out);
    }
    /** get the most recent value
    @return the value*/
    template <class X>
    X getValue ()
    {
        static_assert (((helicsType<X> () != helics_type_t::helicsInvalid) || (isConvertableType<X> ())),
                       "requested types must be one of the primary helics types or convertible to one");
        return getValue_impl<X> (std::conditional_t<(helicsType<X> () != helics_type_t::helicsInvalid),
                                                    std::true_type, std::false_type> ());
    }

    /** get the size of the raw data*/
    size_t getRawSize ();
    /** get the size of the data if it were a string*/
    size_t getStringSize ();
    /** get the number of elements in the data if it were a vector*/
    size_t getVectorSize ();

    // TODO:: add a getValueByReference function that gets the data by reference but may force a copy and will only
    // work on the primary types
    friend class ValueFederateManager;
};

/** class to handle an input and extract a specific type
@tparam X the class of the value associated with a subscription*/
template <class X>
class InputT : public Input
{
  public:
  private:
    std::function<void(X, Time)> value_callback;  //!< callback function for the federate
    std::function<double(const X &v1, const X &v2)>
      changeDetectionOperator;  //!< callback function for change detection
	//determine if we can convert to a primary type
    using is_convertible_to_primary_type =
      std::conditional_t<((helicsType<X> () != helics_type_t::helicsInvalid) || (isConvertableType<X> ())),
                         std::true_type,
                         std::false_type>;
  public:
    InputT () = default;
    /**constructor to build a subscription object
    @param[in] valueFed  the ValueFederate to use
    @param[in] name the name of the input
    @param[in] units the units associated with a Federate
    */
    InputT (ValueFederate *valueFed, const std::string &name, const std::string &units = std::string ())
        : Input (valueFed, name, ValueConverter<X>::type (), units)
    {
    }
    /**constructor to build a subscription object
    @param[in] valueFed  the ValueFederate to use
    @param[in] name the name of the input
    @param[in] units the units associated with a Federate
    */
    template <class FedPtr>
    InputT (FedPtr &valueFed, const std::string &name, const std::string &units = std::string ())
        : Input (valueFed, name, ValueConverter<X>::type (), units)
    {
    }

  private:
    void getValueX_impl (std::true_type /*V*/, X &out) { Input::getValue (out); }
    void getValueX_impl (std::false_type /*V*/, X &out) { fed->getValue (*this, out); }
    X getValueX_impl (std::true_type /*V*/) { return Input::getValue<X> (); }
    X getValueX_impl (std::false_type /*V*/) { return fed->getValue<X> (*this); }

  public:
    /** get the most recent value
    @return the value*/
    X getValue ()
    {
        return getValueX_impl (is_convertible_to_primary_type());
    }
    /** store the value in the given variable
    @param[out] out the location to store the value
    */
    void getValue (X &out)
    {
        getValueX_impl(is_convertible_to_primary_type (),out);
    }

    /** register a callback for the update
    @details the callback is called in the just before the time request function returns
    @param[in] callback a function with signature void(X val, Time time)
    val is the new value and time is the time the value was updated
    */
    void setInputNotificationCallback (std::function<void(X, Time)> callback)
    {
        value_callback = callback;
        fed->setInputNotificationCallback (*this, [=](Input &, Time time) { handleCallback (time); });
    }
    /** set a default value
    @param val the value to set as the default
    */
    void setDefault (const X &val) { fed->setDefaultValue (*this, val); }

    /** store the value in the given variable
    @param[out] out the location to store the value
    */
    const X &operator[] (int index) const { return fed->getValue<X> (*this, index); }

  private:
    void handleCallback (Time time)
    {
        X out;
        fed->getValue (*this, out);
        value_callback (out, time);
    }
};

}  // namespace helics

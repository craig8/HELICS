/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.java.helics;

public final class helics_filter_type {
  public final static helics_filter_type helics_filter_type_custom = new helics_filter_type("helics_filter_type_custom", helicsJNI.helics_filter_type_custom_get());
  public final static helics_filter_type helics_filter_type_delay = new helics_filter_type("helics_filter_type_delay", helicsJNI.helics_filter_type_delay_get());
  public final static helics_filter_type helics_filter_type_random_delay = new helics_filter_type("helics_filter_type_random_delay", helicsJNI.helics_filter_type_random_delay_get());
  public final static helics_filter_type helics_filter_type_random_drop = new helics_filter_type("helics_filter_type_random_drop", helicsJNI.helics_filter_type_random_drop_get());
  public final static helics_filter_type helics_filter_type_reroute = new helics_filter_type("helics_filter_type_reroute", helicsJNI.helics_filter_type_reroute_get());
  public final static helics_filter_type helics_filter_type_clone = new helics_filter_type("helics_filter_type_clone", helicsJNI.helics_filter_type_clone_get());
  public final static helics_filter_type helics_filter_type_firewall = new helics_filter_type("helics_filter_type_firewall", helicsJNI.helics_filter_type_firewall_get());

  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static helics_filter_type swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + helics_filter_type.class + " with value " + swigValue);
  }

  private helics_filter_type(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private helics_filter_type(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private helics_filter_type(String swigName, helics_filter_type swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static helics_filter_type[] swigValues = { helics_filter_type_custom, helics_filter_type_delay, helics_filter_type_random_delay, helics_filter_type_random_drop, helics_filter_type_reroute, helics_filter_type_clone, helics_filter_type_firewall };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}

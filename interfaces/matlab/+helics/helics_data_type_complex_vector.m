function v = helics_data_type_complex_vector()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 1464812639);
  end
  v = vInitialized;
end

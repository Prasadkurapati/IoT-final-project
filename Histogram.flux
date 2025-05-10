from(bucket: "sound_data")
  |> range(start: -6h)
  |> filter(fn: (r) => r["_field"] == "value")
  |> histogram(bins: [0.0, 50.0, 100.0, 200.0, 300.0, 500.0, 800.0, 1000.0, 1500.0, 5000.0])

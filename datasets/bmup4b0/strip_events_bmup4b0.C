//-----------------------------------------------------------------------------
// strip events from bmup4b0s56r0100
// assume stnana is loaded
//-----------------------------------------------------------------------------
int strip_events(int I1, int I2) {
  for (int i=I1; i<=I2; i++) {
    TString fset = Form("%06i",i);
    stnana("pipenu","bmup4b0s56r0100",fset.Data(),"","murat_strip_tracks(\"pipenu\",\"bmup4b0s66r0100\")");
  }
  return 0;
}

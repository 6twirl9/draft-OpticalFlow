from . import _opticalflow
import yaml

class OpticalFlow(_opticalflow.OpticalFlow):

 def available(self):

  return { "device": [ "CUDA", "CPU" ], "algorithm": [ "DualTVL1", "Farneback", "PyrLK" ], "feature": [ "dense" ] }

 def availablePP(self):

  _ = self.available()

  for k, v in self.available().items():

   print(f'{k:16s} : '," , ".join(v))

 def infoPP(self,all=True):

  print(yaml.dump(super().info(all),default_flow_style=False))

  return self


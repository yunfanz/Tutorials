import numpy as np
def lst_grid(lsts, data, wgts=None, lstbins=6300, wgtfunc=lambda dt,res: np.exp(-dt**2/(2*res**2))):
    lstgrid = np.linspace(0, 2*np.pi, lstbins)
    lstres = lstgrid[1]-lstgrid[0]
    if wgts is None: wgts = np.where(np.abs(data) == 0, 0, 1.)
    sumgrid,wgtgrid = 0, 0
    for lst,d,w in zip(lsts,data,wgts):
        dt = lstgrid - lst
        wf = wgtfunc(dt,lstres); wf.shape = (-1,) + (1,)*(data.ndim-1)
        d.shape, w.shape = (1,-1), (1,-1)
        #print lst, dt
        wgtgrid += w * wf
        sumgrid += d * w * wf 
    datgrid = np.where(wgtgrid > 1e-10, sumgrid/wgtgrid, 0)
    #print('real', datgrid)
    return lstgrid, datgrid, wgtgrid
lsts = np.linspace(0, 6.28, 100, dtype=np.float32)
data = np.random.rand(100, 51).astype(np.float32)
def execute():
    aa,bb,cc = lst_grid(lsts, data,lstbins=100)
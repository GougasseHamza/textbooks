SCENARIO = {"duration": 5000}
def check(tr, H):
    return H.mode_is(tr, 5, "OUTPUT") + H.periodic(tr, 5, 100, 900)

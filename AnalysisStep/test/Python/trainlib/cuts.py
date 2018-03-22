def no_cut(row):
    return True

def mZZ_cut(row):
    if row["ZZMass"] >= 118.0 and row["ZZMass"] < 130.0:
        return True
    else:
        return False

def countNeutrinos(row):
    number_neutrinos = 0
    associated_particles = row["LHEAssociatedParticleId"]
    
    if associated_particles is not None:
        for pdg_code in associated_particles:
            if abs(pdg_code) == 12 or abs(pdg_code) == 14 or abs(pdg_code) == 16:
                number_neutrinos += 1
                
    return number_neutrinos

def countAssocLeptons(row):
    if row["GenAssocLep1Id"] == 0 and row["GenAssocLep2Id"] == 0:
        return 0
    elif (row["GenAssocLep1Id"] != 0 and row["GenAssocLep2Id"] == 0) or (row["GenAssocLep1Id"] == 0 and row["GenAssocLep2Id"] != 0):
        return 1
    elif row["GenAssocLep1Id"] != 0 and row["GenAssocLep2Id"] != 0:
        return 2
    
    return 0



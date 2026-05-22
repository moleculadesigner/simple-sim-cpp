import random
import json
import argparse


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="GenSystem",
        description="Generate random system config with given parameters",
        epilog="Dirty"
    )
    parser.add_argument(
        "-n", "--n-particles",
        type=int,
    )
    parser.add_argument(
        "-s", "--n-steps",
        type=int,
    )
    parser.add_argument(
        "-d", "--dump",
        type=int,
    )
    parser.add_argument(
        "-t", "--dt",
        type=float,
    )
    parser.add_argument(
        "--suns",
        action="store_true"
    )
    parser.add_argument(
        "--com",
        action="store_true"
    )
    
    args = parser.parse_args()
    
    if args.suns and args.n_particles > 2:
        suns = [
            {"x": -40.0, "y": -40.0, "z": 0.0, "m": 10_000, "vx": 0.0, "vy": 5.0e-5, "vz": 0.0},
            {"x": 40.0, "y": 40.0, "z": 0.0, "m": 10_000, "vx": 0.0, "vy": -5.0e-5, "vz": 0.0},
        ]
        n_pat = args.n_particles - 2
    elif args.n_particles > 0:
        suns = []
        n_pat = args.n_particles
    else:
        raise ValueError("N < 1")
    
    particles = suns + [
        {
            "x": 200 * (random.random() - 0.5),
            "y": 200 * (random.random() - 0.5),
            "z": 200 * (random.random() - 0.5),
            "m": 99 * random.random() + 1,
        }
        for _ in range(n_pat)
    ]
    
    system = {
        "particles": particles
    }
    config = {
        "dt": args.dt,
        "n_steps": args.n_steps,
        "nst_dump": args.dump,
        "update_com": args.com,
        "system": system,
    }
    
    print(json.dumps(config))
    
#ifndef LBM_TURBULENCEMODEL_H
#define LBM_TURBULENCEMODEL_H

//此头文件参考double-time physical based animation附带的源码


const double CSmago = 0.04;

double lesCoeffOffdiag[2][9];
double lesCoeffDiag[2][9];

extern const double w_l[9];
extern const int e_x[9];
extern const int e_y[9];


extern const double omga;

extern const double ldc_velocity;


inline double getLesNoneqTensorCoeff(std::vector<double>& df, double feq[]) {
	double Qo = 0.0;
	for (int m = 0; m < 2; m++) {
		double qadd = 0.0;
		for (int l = 0; l < 9; l++) {
			if (lesCoeffOffdiag[m][l] == 0.0) continue;
			qadd += lesCoeffOffdiag[m][l] * (df[l] - feq[l]);
		}
		Qo += (qadd * qadd);
		for (int m = 0; m < 2; m++) {
			double qadd = 0.0;
			for (int l = 0; l < 9; l++) {
				if (lesCoeffDiag[m][l] == 0.0) continue;
				qadd += lesCoeffDiag[m][l] * (df[l] - feq[l]);
			}
			Qo += (qadd * qadd);
		}
		Qo = sqrt(Qo);
		return Qo;
	}
}


inline double getLesOmega(double omega, double csmago, double Qo) {
	const double tau = 1.0 / omega;
	const double nu = (2.0 * tau - 1.0) * (1.0 / 6.0);
	const double C = csmago;
	const double Csqr = C * C;
	double S = -nu + sqrt(nu * nu + 18.0 * Csqr * Qo) / (6.0 * Csqr);
	//cout << Qo << endl;
	return(1.0 / (3.0 * (nu + Csqr * S) + 0.5));
}


inline void collideSmagorinsky(std::vector<double>& df, int flag) {
	int l;
	double rho = df[0];
	double u_x = 0;
	double u_y = 0;
	double feq[9];
	double omegaNew;
	double Qo = 0.0;

	// just like normal collision...
	for (l = 1; l < 9; l++) {
		rho += df[l];
		u_x += e_x[l] * df[l];
		u_y += e_y[l] * df[l];
	}

	if (flag == LBM_VELOCITY) {
		u_x = ldc_velocity;
	}

	u_x /= rho;
	u_y /= rho;

	for (l = 0; l < 9; l++) {
		const double a = e_x[l] * u_x + e_y[l] * u_y;
		feq[l] = w_l[l] * rho * (1 + 3 * a + 4.5 * a * a - 1.5 * (u_x * u_x + u_y * u_y));
	}

	// new - compute the new local viscosity
	// according to magnitude of the local Reynolds
	// stress tensor
	Qo = getLesNoneqTensorCoeff(df, feq);

	omegaNew = getLesOmega(omga, CSmago, Qo);

	// relax with new omega...
	for (l = 0; l < 9; l++) {
		df[l] = (1.0 - omegaNew) * df[l] + omegaNew * feq[l];
	}
};



// init helper function
inline void initSmagoArrays() {
	// init Smagorinsky model (precompute 
	// stress tensor factors)

	int odm = 0;
	for (int m = 0; m < 2; m++) {
		for (int l = 0; l < 9; l++) {
			lesCoeffDiag[m][l] = lesCoeffOffdiag[m][l] = 0.0;
		}
	}

	for (int m = 0; m < 2; m++) {
		for (int n = 0; n < 2; n++) {
			for (int l = 0; l < 9; l++) {
				double em;
				switch (m) {
				case 0: em = e_x[l]; break;
				case 1: em = e_y[l]; break;
				}
				double en;
				switch (n) {
				case 0: en = e_x[l]; break;
				case 1: en = e_y[l]; break;
				}
				const double coeff = em * en;
				if (m == n) {
					lesCoeffDiag[m][l] = coeff;
				}
				else {
					if (m > n) {
						lesCoeffOffdiag[odm][l] = coeff;
					}
				}
			}

			if (m == n) {
			}
			else {
				if (m > n) odm++;
			}
		}
	}


};

#endif
void Excitation() {
	TF1 *fExcitation = new TF1("Excitation", "gaus", 0, 100);
	fExcitation->SetParameters(1, 50, 10);
	for(Double_t i=0.5; i<100; i=i+0.5) {
		std::cout << i << " " << fExcitation->Eval(i) << std::endl;
	}
	fExcitation->Draw();
}

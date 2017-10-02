#include "DetailScene.h"

void DetailScene::setup(HandCursor *hc) {
	this->icons.insert(make_pair("return", Icon(this->window_width - 200, 100, 100, 100, "C:/of_v0.9.8_vs_release/apps/myApps/DigitalSignage/fig/return.jpeg")));

	this->hc = hc;

	font.loadFont("meiryob.ttc", 30);
}

void DetailScene::update() {
	bool flag;
	for (auto &i : this->icons) {
		flag = false;
		for (auto &t : this->hc->track_data) {
			if (i.second.is_inside(ofPoint(t.second.current_pointer.x, t.second.current_pointer.y))) {
				pair<string, int> id(i.first, t.first); // アイコン名と手ポインタidの情報
				ofNotifyEvent(this->point_event, id);
				flag = true;
				break;
			}
		}
		if (!flag) {
			i.second.change_state("None");
		}
		i.second.update();
	}
	/* 手ポインタがアイコンに触れたらイベントを発火 */
	/*
	for (auto t : this->hc->track_data) {
		for (auto i : this->icons) {
			if (i.second.is_inside(ofPoint(t.second.current_pointer.x, t.second.current_pointer.y))) {
				pair<string, int> id(i.first, t.first); // アイコン名と手ポインタidの情報
				ofNotifyEvent(this->point_event, id);
			}
		}
	}
	*/
}

void DetailScene::draw() {
	for (auto &i : this->icons) {
		i.second.draw();
	}

	ofSetColor(30, 200, 200); // 文字色を指定
	font.drawString(L"\n\n\n\n水素水（すいそすい）は、水素分子のガスを溶解させた水であり、無味、無臭、無色である。\n水素は水にごくわずかに溶ける。工業用の水素水は半導体や液晶の洗浄に用いられる。\n農業では作物成長や食品保存での研究が行われている。\nまた飲用のアルカリ性電解水の生成に伴い水素水が生成される\n（水素水ではなくアルカリイオン水生成装置には「胃腸症状の改善」の効能表示が認められている）。\nほかに市販の飲料水も販売されているが、水素が含まれない商品も少数あり、\nまた効能表示に関して、国民生活センターによる表示できないことの注意警告と、\n効果がないと誤解されているという業者の意見とで食い違いが生じている。\n研究結果の共通性から水素と水素水の抗酸化作用には疑う余地はないが、\n作用機所はまだ明確には特定されていない。\n特に2007年以降に水素に関する医学的な研究が日本、アメリカ、中国にて増加し、\n2015年6月までに水素水の臨床試験は14研究報告され有意な効果がみられている。\n電解水素水では、パーキンソン病患者での二重盲検法試験、\n人工透析の際の透析液としての利用研究がある。\n2016年5月には、国立健康・栄養研究所は、6つのランダム化比較試験を元に、\nほとんどが病気の患者での予備的研究であるため、\n健康な人への有効性について「信頼できる十分なデータが見当たらない」としている。", 100, 100);
}
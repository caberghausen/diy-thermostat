//
//  ThermostatView.swift
//  MottThermostat
//
//  Created by Charles Berghausen on 6/3/24.
//

import Combine
import SwiftUI

struct ThermostatView: View {
    private let viewModel = ThermostatViewModel()

    var body: some View {
        VStack {
            Button(action :{
                viewModel.toggleAC()
            }) {
                Text(viewModel.toggleButtonText)
                    .font(.title)
            }
            .padding()
        }
        .padding()
    }
}

#Preview {
    ThermostatView()
}

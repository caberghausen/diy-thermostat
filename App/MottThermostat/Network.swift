//
//  Network.swift
//  MottThermostat
//
//  Created by Charles Berghausen on 6/3/24.
//

import Foundation

enum ThermostatMode: String, Decodable {
    case onUntilStopped
    case scheduled
}

enum ThermostatSetting: String, Decodable {
    case on
    case off
}

struct ThermostatResponse: Decodable {
    let mode: ThermostatMode
    let thermostatSetting: ThermostatSetting
    let nextUpdateTime: String?
}

final class Network {

    private enum Constants {
        static let networkRequestTimeout: Double = 5
        /// The root endpoint of the ESP8266:
        static let esp8266IPAddress = "http://192.168.0.134/"
    }

    private let jsonDecoder = JSONDecoder()

    private lazy var urlSession: URLSession = {
        var sessionConfig = URLSessionConfiguration.default
        sessionConfig.timeoutIntervalForRequest = Constants.networkRequestTimeout
        return URLSession(configuration: sessionConfig)
    }()

    private func makeNetworkRequest(request: URLRequest) async throws -> ThermostatResponse {
        let responseData = try await urlSession.data(for: request)
        print("Data received: \(responseData)")
        let jsonDecodedData = try jsonDecoder.decode(ThermostatResponse.self, from: responseData.0)
        print("Decoded JSON: \(jsonDecodedData)")
        return jsonDecodedData
    }

    func getCurrentACMode() async throws -> ThermostatResponse {
        let endpoint = URL(string: Constants.esp8266IPAddress)!
        let getTemperatureURLRequest = URLRequest(url: endpoint)
        print("Starting getACMode query...")
        return try await makeNetworkRequest(request: getTemperatureURLRequest)
    }

    func setAC(to setting: ThermostatSetting) async throws -> ThermostatResponse {
        let endpointString: String = Constants.esp8266IPAddress + (setting == .on ? "turnOn" : "turnOff")
        let endpoint = URL(string: endpointString)!
        let setAcURLRequest = URLRequest(url: endpoint)
        print("Starting setAC(to: \(setting)) query...")
        return try await makeNetworkRequest(request: setAcURLRequest)
    }
}
